#ifndef MULTITOUCHINTERACTIVE_H
#define MULTITOUCHINTERACTIVE_H

#include "TuioCursor.h"
#include "HitRectangle.h"

class MultiTouchInteractive : public HitRectangle
{
    public:
        MultiTouchInteractive() {}
        virtual ~MultiTouchInteractive() {}

        virtual void touchDown ( TuioCursor & tcur ) { }
        virtual void touchUpdate ( TuioCursor & tcur ) { }
        virtual void touchUp ( TuioCursor & tcur ) { }

    protected:
    private:
};

#endif // MULTITOUCHINTERACTIVE_H
