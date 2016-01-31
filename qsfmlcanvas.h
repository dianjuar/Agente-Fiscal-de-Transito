#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

#include "headers/qt_stuff.h"
#include "headers/sfml_stuff.h"

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
//Q_OBJECT
public:

    explicit QSFMLCanvas(QWidget *parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

    virtual void            showEvent(QShowEvent*);
    virtual QPaintEngine*   paintEngine() const;
    virtual void            paintEvent(QPaintEvent*);
    virtual                 ~QSFMLCanvas();
    virtual void            OnInit() = 0;
    virtual void            update(float deltaTime, float currentTime) = 0;
    virtual void            draw() = 0;


    void OnUpdate();
    void reset_currentTime();
    void setBackgroudColor( sf::Color backgroudColor );

    float get_currentTime();

protected:
    sf::Color backgroudColor;
    sf::Clock clock;
    float currentTime;

private:
    QTimer myTimer;
    bool myInitialized;

    void updateGame();
    void drawGame();
};

#endif // QSFMLCANVAS_H
