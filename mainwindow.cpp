#include "mainwindow.h"
#include <QGroupBox>
#include <QFont>
#include <QHeaderView>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Stock Portfolio Tracker");
    resize(900, 650);
    
    setupUI();
    
    // Load initial data
    transactions = loadTransactions();
    portfolio = aggregatePortfolio(transactions);
    updatePortfolioTable();
    updateSummaryLabels();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Title
    QLabel *titleLabel = new QLabel("Stock Portfolio Tracker", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    buyButton = new QPushButton("Buy Stock", this);
    sellButton = new QPushButton("Sell Stock", this);
    refreshButton = new QPushButton("Refresh Prices", this);
    historyButton = new QPushButton("Transaction History", this);
    
    buyButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-size: 14px; }");
    sellButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 10px; font-size: 14px; }");
    refreshButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 10px; font-size: 14px; }");
    historyButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 10px; font-size: 14px; }");
    
    buttonLayout->addWidget(buyButton);
    buttonLayout->addWidget(sellButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(historyButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Portfolio Table
    QGroupBox *portfolioGroup = new QGroupBox("Current Portfolio", this);
    QVBoxLayout *portfolioLayout = new QVBoxLayout(portfolioGroup);
    
    portfolioTable = new QTableWidget(this);
    portfolioTable->setColumnCount(6);
    portfolioTable->setHorizontalHeaderLabels({"Symbol", "Quantity", "Avg Price", "Current Price", "Value", "Gain/Loss"});
    portfolioTable->horizontalHeader()->setStretchLastSection(true);
    portfolioTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    portfolioTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    portfolioLayout->addWidget(portfolioTable);
    mainLayout->addWidget(portfolioGroup);
    
    // Summary Labels
    QGroupBox *summaryGroup = new QGroupBox("Portfolio Summary", this);
    QVBoxLayout *summaryLayout = new QVBoxLayout(summaryGroup);
    
    totalValueLabel = new QLabel("Total Portfolio Value: $0.00", this);
    totalCostLabel = new QLabel("Total Cost Basis: $0.00", this);
    totalGainLossLabel = new QLabel("Total Gain/Loss: $0.00", this);
    returnLabel = new QLabel("Return: 0.00%", this);
    
    QFont summaryFont;
    summaryFont.setPointSize(11);
    summaryFont.setBold(true);
    
    totalValueLabel->setFont(summaryFont);
    totalCostLabel->setFont(summaryFont);
    totalGainLossLabel->setFont(summaryFont);
    returnLabel->setFont(summaryFont);
    
    summaryLayout->addWidget(totalValueLabel);
    summaryLayout->addWidget(totalCostLabel);
    summaryLayout->addWidget(totalGainLossLabel);
    summaryLayout->addWidget(returnLabel);
    
    mainLayout->addWidget(summaryGroup);
    
    // Connect signals
    connect(buyButton, &QPushButton::clicked, this, &MainWindow::buyStock);
    connect(sellButton, &QPushButton::clicked, this, &MainWindow::sellStock);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshPortfolio);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::showTransactionHistory);
}

void MainWindow::buyStock() {
    bool ok;
    QString symbol = QInputDialog::getText(this, "Buy Stock", "Enter stock symbol (e.g., AAPL):", QLineEdit::Normal, "", &ok);
    
    if (!ok || symbol.isEmpty()) return;
    
    symbol = symbol.toUpper();
    
    // Validate symbol
    if (symbol.length() > 5) {
        QMessageBox::warning(this, "Invalid Symbol", "Stock symbol must be 1-5 characters.");
        return;
    }
    
    for (QChar c : symbol) {
        if (!c.isLetter()) {
            QMessageBox::warning(this, "Invalid Symbol", "Stock symbol must contain only letters.");
            return;
        }
    }
    
    int quantity = QInputDialog::getInt(this, "Buy Stock", "Enter quantity:", 1, 1, 1000000, 1, &ok);
    if (!ok) return;
    
    double price = QInputDialog::getDouble(this, "Buy Stock", "Enter purchase price:", 0.0, 0.01, 1000000.0, 2, &ok);
    if (!ok) return;
    
    // Log transaction
    logTransaction("BUY", symbol.toStdString(), quantity, price);
    
    // Reload data
    transactions = loadTransactions();
    portfolio = aggregatePortfolio(transactions);
    updatePortfolioTable();
    updateSummaryLabels();
    
    QMessageBox::information(this, "Success", QString("Successfully bought %1 shares of %2 at $%3")
                            .arg(quantity).arg(symbol).arg(price, 0, 'f', 2));
}

void MainWindow::sellStock() {
    bool ok;
    QString symbol = QInputDialog::getText(this, "Sell Stock", "Enter stock symbol (e.g., AAPL):", QLineEdit::Normal, "", &ok);
    
    if (!ok || symbol.isEmpty()) return;
    
    symbol = symbol.toUpper();
    
    // Check if user owns this stock
    bool found = false;
    int availableShares = 0;
    for (const auto& stock : portfolio) {
        if (stock.symbol == symbol.toStdString()) {
            found = true;
            availableShares = stock.quantity;
            break;
        }
    }
    
    if (!found) {
        QMessageBox::warning(this, "Error", QString("You don't own any shares of %1.").arg(symbol));
        return;
    }
    
    int quantity = QInputDialog::getInt(this, "Sell Stock", 
                                        QString("Enter quantity (Available: %1):").arg(availableShares), 
                                        1, 1, availableShares, 1, &ok);
    if (!ok) return;
    
    double price = QInputDialog::getDouble(this, "Sell Stock", "Enter sale price:", 0.0, 0.01, 1000000.0, 2, &ok);
    if (!ok) return;
    
    // Log transaction
    logTransaction("SELL", symbol.toStdString(), quantity, price);
    
    // Reload data
    transactions = loadTransactions();
    portfolio = aggregatePortfolio(transactions);
    updatePortfolioTable();
    updateSummaryLabels();
    
    QMessageBox::information(this, "Success", QString("Successfully sold %1 shares of %2 at $%3")
                            .arg(quantity).arg(symbol).arg(price, 0, 'f', 2));
}

void MainWindow::refreshPortfolio() {
    portfolio = aggregatePortfolio(transactions);
    updatePortfolioTable();
    updateSummaryLabels();
    QMessageBox::information(this, "Refreshed", "Portfolio prices have been updated.");
}

void MainWindow::showTransactionHistory() {
    if (transactions.empty()) {
        QMessageBox::information(this, "Transaction History", "No transactions yet.");
        return;
    }
    
    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Transaction History");
    historyDialog->resize(700, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(historyDialog);
    
    QTableWidget *table = new QTableWidget(historyDialog);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Date", "Action", "Symbol", "Quantity", "Price"});
    table->setRowCount(transactions.size());
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    int row = 0;
    for (const auto& txn : transactions) {
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(txn.date)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(txn.action)));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(txn.symbol)));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(txn.quantity)));
        table->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(txn.price, 0, 'f', 2)));
        row++;
    }
    
    layout->addWidget(table);
    
    QPushButton *closeButton = new QPushButton("Close", historyDialog);
    connect(closeButton, &QPushButton::clicked, historyDialog, &QDialog::accept);
    layout->addWidget(closeButton);
    
    historyDialog->exec();
    delete historyDialog;
}

void MainWindow::updatePortfolioTable() {
    portfolioTable->setRowCount(0);
    
    for (auto& stock : portfolio) {
        int row = portfolioTable->rowCount();
        portfolioTable->insertRow(row);
        
        // Get current price
        double price = getStockPrice(stock.symbol, stock.lastFetched);
        stock.currentPrice = price;
        
        double value = (price > 0) ? price * stock.quantity : 0.0;
        double cost = stock.avgPurchasePrice * stock.quantity;
        double gainLoss = value - cost;
        
        portfolioTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(stock.symbol)));
        portfolioTable->setItem(row, 1, new QTableWidgetItem(QString::number(stock.quantity)));
        portfolioTable->setItem(row, 2, new QTableWidgetItem(QString("$%1").arg(stock.avgPurchasePrice, 0, 'f', 2)));
        
        if (price > 0) {
            portfolioTable->setItem(row, 3, new QTableWidgetItem(QString("$%1").arg(price, 0, 'f', 2)));
            portfolioTable->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(value, 0, 'f', 2)));
            portfolioTable->setItem(row, 5, new QTableWidgetItem(QString("%1$%2")
                                                                 .arg(gainLoss >= 0 ? "+" : "-")
                                                                 .arg(std::abs(gainLoss), 0, 'f', 2)));
            
            // Color code gain/loss
            QTableWidgetItem *gainLossItem = portfolioTable->item(row, 5);
            if (gainLoss >= 0) {
                gainLossItem->setForeground(QBrush(QColor(0, 128, 0)));
            } else {
                gainLossItem->setForeground(QBrush(QColor(255, 0, 0)));
            }
        } else {
            portfolioTable->setItem(row, 3, new QTableWidgetItem("Error"));
            portfolioTable->setItem(row, 4, new QTableWidgetItem("N/A"));
            portfolioTable->setItem(row, 5, new QTableWidgetItem("N/A"));
        }
    }
    
    portfolioTable->resizeColumnsToContents();
}

void MainWindow::updateSummaryLabels() {
    double totalValue = 0.0;
    double totalCost = 0.0;
    
    for (const auto& stock : portfolio) {
        if (stock.currentPrice > 0) {
            totalValue += stock.currentPrice * stock.quantity;
            totalCost += stock.avgPurchasePrice * stock.quantity;
        }
    }
    
    double totalGainLoss = totalValue - totalCost;
    double returnPct = (totalCost > 0) ? (totalGainLoss / totalCost) * 100 : 0.0;
    
    totalValueLabel->setText(QString("Total Portfolio Value: $%1").arg(totalValue, 0, 'f', 2));
    totalCostLabel->setText(QString("Total Cost Basis: $%1").arg(totalCost, 0, 'f', 2));
    
    QString gainLossText = QString("Total Gain/Loss: %1$%2")
                          .arg(totalGainLoss >= 0 ? "+" : "-")
                          .arg(std::abs(totalGainLoss), 0, 'f', 2);
    totalGainLossLabel->setText(gainLossText);
    
    if (totalGainLoss >= 0) {
        totalGainLossLabel->setStyleSheet("QLabel { color: green; }");
    } else {
        totalGainLossLabel->setStyleSheet("QLabel { color: red; }");
    }
    
    QString returnText = QString("Return: %1%2%")
                        .arg(returnPct >= 0 ? "+" : "")
                        .arg(returnPct, 0, 'f', 2);
    returnLabel->setText(returnText);
    
    if (returnPct >= 0) {
        returnLabel->setStyleSheet("QLabel { color: green; }");
    } else {
        returnLabel->setStyleSheet("QLabel { color: red; }");
    }
}
