#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QClipboard>
#include <QGridLayout>
#include <QMouseEvent>
#include <QWebView>
#include <QLabel>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void doClipboardChanged();
    void replyFinished(QNetworkReply*);

private:
    bool eventFilter(QObject *, QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    int width, height, quit_size;
    bool mousepressed;
    QGridLayout *layout;
    QClipboard *clip;
    QWebView *web;
    QLabel *label;
    QNetworkAccessManager *manager;
};

#endif // WIDGET_H
