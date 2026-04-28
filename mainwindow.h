#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QTimer>
#include <vector>
#include "stock.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void buyStock();
    void sellStock();
    void refreshPortfolio();
    void showTransactionHistory();

private:
    void setupUI();
    void updatePortfolioTable();
    void updateSummaryLabels();
    
    // Data
    std::vector<Transaction> transactions;
    std::vector<Stock> portfolio;
    
    // UI Components
    QTableWidget *portfolioTable;
    QTableWidget *transactionTable;
    QPushButton *buyButton;
    QPushButton *sellButton;
    QPushButton *refreshButton;
    QPushButton *historyButton;
    QLabel *totalValueLabel;
    QLabel *totalCostLabel;
    QLabel *totalGainLossLabel;
    QLabel *returnLabel;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
};

#endif // MAINWINDOW_H
