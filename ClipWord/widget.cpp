#include <QApplication>
#include <QWebFrame>
#include <QWebElementCollection>
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    width = 450;
    height = 300;
    quit_size = 25;
    setFixedSize(width,height);
    setWindowFlags(Qt::FramelessWindowHint);

    clip = QApplication::clipboard();
    connect(clip,SIGNAL(dataChanged()),this,SLOT(doClipboardChanged()));

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    layout = new QGridLayout(this);
    layout->setMargin(0);

    web = new QWebView(this);
    web->installEventFilter(this);
    web->setUrl(QString("qrc:/index.html"));

    QPixmap image("://quit.png");
    label = new QLabel();
    label->installEventFilter(this);
    label->setFixedSize(quit_size,quit_size);
    label->setPixmap(image);
    label->setScaledContents(true);

    layout->addWidget(web,0,0,1,2);
    layout->setAlignment(web,Qt::AlignCenter|Qt::AlignAbsolute);
    layout->addWidget(label,0,1);
    layout->setAlignment(label,Qt::AlignTop|Qt::AlignRight);

    mousepressed = false;
}

Widget::~Widget()
{
    delete label;
    delete web;
    delete layout;
    delete manager;
    delete clip;
}

void Widget::doClipboardChanged(){
    QString txt = clip->text().trimmed();
    QRegExp regex("\\s?[a-zA-Z]{1,32}\\s?\\.?,?");
    if(regex.exactMatch(txt)){
        txt = txt.replace(QRegExp("\\.$|,$"),"");
        txt = txt.trimmed();
        qDebug(txt.toLatin1());
        manager->get(QNetworkRequest(QUrl("http://open.iciba.com/huaci/dict.php?word="+txt)));
    }else
        qDebug("false");
}

void Widget::replyFinished(QNetworkReply *reply){
    if(reply->error()) {
        web->page()->mainFrame()->evaluateJavaScript("networkError();");
        activateWindow();
        return;
    }
    QString js = QString(reply->readAll());
    if(!js.contains("icIBahyI-main_cont")){
        web->page()->mainFrame()->evaluateJavaScript("networkError();");
        activateWindow();
        return;
    }
    js = js.remove(0,js.indexOf("='")+2);
    js.truncate(js.indexOf("';"));
    QString script = QString("getAjaxData('%1');").arg(js);
    web->page()->mainFrame()->evaluateJavaScript(script);
    activateWindow();
}

bool Widget::eventFilter(QObject *obj, QEvent *e){
    if(obj==web){
        if(e->type()==QEvent::MouseButtonPress){
            mousePressEvent((QMouseEvent*)e);
        }else if(e->type()==QEvent::MouseButtonRelease){
            mouseReleaseEvent((QMouseEvent*)e);
        }else if(e->type()==QEvent::MouseMove){
            mouseMoveEvent((QMouseEvent*)e);
        }
    }
    else if(obj==label){
        if(e->type()==QEvent::MouseButtonPress){
            closeEvent((QCloseEvent *)e);
        }
    }
    return QWidget::eventFilter(obj,e);
}

void Widget::mousePressEvent(QMouseEvent *event) {
    mousepressed = true;
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    event->accept();
    mousepressed = false;
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if(mousepressed)
        move(event->globalX()-m_nMouseClick_X_Coordinate,
             event->globalY()-m_nMouseClick_Y_Coordinate);
    else{
        int x = event->globalX()-this->pos().x();
        int y = event->globalY()-this->pos().y();
        if(x>width-quit_size&&y<quit_size)
            label->show();
        else if(!label->isHidden())
            label->hide();
    }
}

void Widget::closeEvent(QCloseEvent *event){
    event->accept();
    this->close();
}
