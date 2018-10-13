#pragma once

#include <ray/components/Movable.hpp>
#include <ray/components/Orientable.hpp>
#include <ray/math/LinearAlgebra.hpp>
#include <ray/platform/Window.hpp>
#include <ray/platform/Inputs.hpp>
#include <ray/math/Transform.hpp>

#ifdef WIN32
#undef near
#undef far
#endif

namespace ray { namespace entities {

    class Camera : public components::Movable, public components::Orientable
    {
        using rad = math::rad;
        using mat4 = math::mat4;
        using Window = platform::Window;
        using Key = platform::Key;
        using MouseButton = platform::MouseButton;
        using dvec2 = math::dvec2;
        using vec3 = math::vec3;

        rad mFieldOfView;    
        mat4 mProjection, mView;
        float mMovementSpeed, mMouseSensitivity;
    public:
    
        Camera(rad fov, float aspect, float near, float far) : mFieldOfView(fov), mMovementSpeed(2.5f), mMouseSensitivity(0.001f)
        {
            mProjection = math::perspective(fov, aspect, near, far);
            mView = lookAt(position(), position() + front(), up());
        }
    
        const mat4 &viewMatrix()       const { return mView; }
        const mat4 &projectionMatrix() const { return mProjection; }
    
        void update(const Window &window, float dt)
        {        
            auto moved = moveUsingKeyboard(window, dt);
            auto looked = lookUsingMouse(window);
            auto zoomed = zoomUsingScrollWheel(window);
    
            if (moved || looked) mView       = lookAt(position(), position() + front(), up());
            if (zoomed)          mProjection = math::perspective(mFieldOfView, window.aspectRatio(),  0.1f, 100.0f);        
        }
    
    private:
        bool moveUsingKeyboard(const Window &window, float dt)
        {
            float amount = mMovementSpeed * dt;
            if (amount)
            {
                if (window.isKeyHeld(Key::KEY_UP))        move(front(), amount);
                if (window.isKeyHeld(Key::KEY_DOWN))      move(back(),  amount);
                if (window.isKeyHeld(Key::KEY_LEFT))      move(left(),  amount);
                if (window.isKeyHeld(Key::KEY_RIGHT))     move(right(), amount);
                if (window.isKeyHeld(Key::KEY_PAGE_UP))   move(up(),    amount);
                if (window.isKeyHeld(Key::KEY_PAGE_DOWN)) move(down(),  amount);            
                return true;
            }
            return false;
        }
    
        bool lookUsingMouse(const Window &window)
        {
            static dvec2 lastCursorPos;
    
            if (window.isMouseButtonReleased(MouseButton::BUTTON_LEFT))
            {
                window.showMouseCursor();		
            }
            else if (window.isMouseButtonPressed(MouseButton::BUTTON_LEFT))
            {
                window.disableMouseCursor();
                window.getCursorPosition(lastCursorPos.x, lastCursorPos.y);
            }
            else if (window.isMouseButtonHeld(MouseButton::BUTTON_LEFT))
            {
                auto newPos = dvec2();
                window.getCursorPosition(newPos.x, newPos.y);
                auto dpos = lastCursorPos - newPos;
                lastCursorPos = newPos;
                if (dpos.x != 0) rotate(vec3(0,1,0), mMouseSensitivity * dpos.x);
                if (dpos.y != 0) rotate(left(), -mMouseSensitivity * dpos.y);
                return true;
            }
    
            return false;
        }    
    
        bool zoomUsingScrollWheel(const Window &window)
        {
            using namespace math;

            double xOffset, yOffset;
            window.getScrollOffsets(xOffset, yOffset);
            if (yOffset)
            {
                mFieldOfView = clamp(10_deg, mFieldOfView - yOffset*PI_OVER_180, 90_deg);        
                return true;
            }
            return false;
        }
    };
    

}}