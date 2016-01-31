#include "qsfmlcanvas.h"

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) :
             QWidget(Parent),
             myInitialized (false),
             currentTime ( 0.0f )
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the widget geometry
    move(Position);
    resize(Size);

    // Setup the timer
    myTimer.setInterval(FrameTime);
}

QSFMLCanvas::~QSFMLCanvas() {}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
        //XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        //RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        RenderWindow::create( winId());

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();
        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
    OnUpdate();
    // Display on screen
    RenderWindow::display();
}

void QSFMLCanvas::OnUpdate()
{
    updateGame();
    drawGame();
}

void QSFMLCanvas::reset_currentTime()
{
    currentTime = 0;
}

void QSFMLCanvas::setBackgroudColor(sf::Color backgroudColor)
{
    this->backgroudColor = backgroudColor;
}

float QSFMLCanvas::get_currentTime()
{
    return currentTime;
}

void QSFMLCanvas::updateGame()
{
    float deltaTime = clock.restart().asSeconds();
    currentTime += deltaTime;

    update(deltaTime, currentTime);
}

void QSFMLCanvas::drawGame()
{
    clear( backgroudColor );

    //user draw
    draw();

    display();
}


