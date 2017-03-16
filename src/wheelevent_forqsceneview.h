#ifndef WHEELEVENT_FORQSCENEVIEW_H
#define WHEELEVENT_FORQSCENEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class WheelEvent_forQSceneView:public QGraphicsView
{

public:
    WheelEvent_forQSceneView(QWidget* parent = 0):QGraphicsView(parent)
    {

    }
    void wheelEvent(QWheelEvent *event)
    {
      const double scaleFactor = 1.15;
      if(event->delta() > 0)
      {
        // Zoom in
        scale(scaleFactor, scaleFactor);
      }
      else
      {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
      }
    }
};

#endif // WHEELEVENT_FORQSCENEVIEW_H
