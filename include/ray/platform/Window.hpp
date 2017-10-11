#pragma once

#include <ray/platform/Inputs.hpp>
#include <ray/platform/OpenGL.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <bitset>

namespace ray { namespace platform {

    class Window 
    {    
        class GLFW 
        {
            friend class Window;
            inline GLFW() 
            { 
                glfwInit();
                windowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                windowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                windowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                windowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            
            }
        public:
            static inline const GLFW &getInstance() { static GLFW instance; return instance; }
            inline ~GLFW() { glfwTerminate(); }
        private:
            inline void defaultWindowHint() const { glfwDefaultWindowHints(); }
            inline void windowHint(int hint, int value) const { glfwWindowHint(hint, value); }
            inline GLFWwindow *createWindow(int width, int height, std::string title, GLFWmonitor *monitor, GLFWwindow *share) const { return glfwCreateWindow(width, height, title.c_str(), monitor, share); }
            inline GLFWwindow *getCurrentContext() const { return glfwGetCurrentContext(); }
            inline void pollEvents() const { glfwPollEvents(); }
        };


        friend class GLFW;
        GLFWwindow *mHandle;
        bool mShouldBeDestroyed;

        inline Window(GLFWwindow *handle, bool shouldBeDestroyed) : mHandle(handle), mShouldBeDestroyed(shouldBeDestroyed) 
        {
            makeContextCurrent();
            gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
            swapInterval(0);
            setUserPointer<Window>(*this);                    

            setKeyCallback([](GLFWwindow *w, int key, int, int action, int) {
                auto &window = self(w);
                if (action == GLFW_PRESS) 
                {
                    window.mIsKeyPressed[key] = true;
                    window.mIsKeyHeld[key] = true;
                }
                if (action == GLFW_RELEASE)
                {
                    window.mIsKeyReleased[key] = true;
                    window.mIsKeyHeld[key] = false;
                }
            });
        
            setMouseButtonCallback([](GLFWwindow *w, int button, int action, int) {
                auto &window = self(w);
                if (action == GLFW_PRESS)
                {
                    window.mIsMouseButtonPressed[button] = true;
                    window.mIsMouseButtonHeld[button] = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    window.mIsMouseButtonReleased[button] = true;
                    window.mIsMouseButtonHeld[button] = false;
                }
            });

            setFramebufferSizeCallback([](GLFWwindow *win, int w, int h) {                          
                glViewport(0,0,w,h);
            });

            int width, height;
            glfwGetWindowSize(mHandle, &width, &height);
            int frameBufferWidth, frameBufferHeight;
            int winWidth, winHeight;
            getSize(winWidth, winHeight);
            getFrameBufferSize(frameBufferWidth, frameBufferHeight);
            float xScale = (float)frameBufferHeight/winHeight;
            float yScale = (float)frameBufferWidth/winWidth;
            setSize(width/xScale,height/yScale);
            glViewport(0, 0, width, height); 
        }
    public:
        inline Window(int width, int height, const std::string &title, const Window &share) : Window(GLFW::getInstance().createWindow(width, height, title, nullptr, share.mHandle), true) {}
        inline Window(int width, int height, const std::string &title) : Window(GLFW::getInstance().createWindow(width, height, title, nullptr, nullptr), true) {}
        inline Window(const Window &other) = delete;
        inline Window(Window &&other) { mHandle = other.mHandle; other.mHandle = nullptr; }
        inline virtual ~Window() { if (mShouldBeDestroyed) glfwDestroyWindow(mHandle); }
        inline void swapInterval(int interval) const { return glfwSwapInterval(interval); }                
        inline bool shouldClose() const { return glfwWindowShouldClose(mHandle); }        
        inline void setShouldClose(bool value) const { return glfwSetWindowShouldClose(mHandle, value); }
        inline void setTitle(const std::string &title) const { glfwSetWindowTitle(mHandle, title.c_str()); }
        inline void setIcon(const std::vector<GLFWimage> &images) const { glfwSetWindowIcon(mHandle, images.size(), &images[0]); } // @TODO: create proper Image class
        inline void getPosition(int &x, int &y) const { glfwGetWindowPos(mHandle, &x, &y); }
        inline void setPosition(int x, int y) const { glfwSetWindowPos(mHandle, x, y); }
        inline void getSize(int &w, int &h) const { glfwGetWindowSize(mHandle, &w, &h); }
        inline void setSizeLimites(int minWidth, int minHeight, int maxWidth, int maxHeight) const { glfwSetWindowSizeLimits(mHandle, minWidth, minHeight, maxWidth, maxHeight); }
        inline void setAspectRatio(int numerator, int denominator) const { glfwSetWindowAspectRatio(mHandle, numerator, denominator); }
        inline void setSize(int w, int h) const { glfwSetWindowSize(mHandle, w, h); }
        inline void getFrameBufferSize(int &w, int &h) const { glfwGetFramebufferSize(mHandle, &w, &h); }
        inline void getFrameSize(int &left, int &top, int &right, int &bottom) const { glfwGetWindowFrameSize(mHandle, &left, &top, &right, &bottom); }
        inline void iconify() const { glfwIconifyWindow(mHandle); }
        inline void restore() const { glfwRestoreWindow(mHandle); }
        inline void maximize() const { glfwMaximizeWindow(mHandle); }
        inline void show() const { glfwShowWindow(mHandle); }
        inline void hide() const { glfwHideWindow(mHandle); }
        inline void focus() const { glfwFocusWindow(mHandle); }
        inline void setWindowed(int x, int y, int w, int h) const { glfwSetWindowMonitor(mHandle, nullptr, x, y, w, h, 0); }
        inline int  getAttribute(int attribute) const { return glfwGetWindowAttrib(mHandle, attribute); }
        inline void setUserPointer(void *userPointer) const { glfwSetWindowUserPointer(mHandle, userPointer); }
        inline void *getUserPointer() const { return glfwGetWindowUserPointer(mHandle); }
        template<typename T> inline void setUserPointer(T &userPointer) const { glfwSetWindowUserPointer(mHandle, reinterpret_cast<void*>(&userPointer)); }
        template<typename T> inline T &getUserPointer() const { return *reinterpret_cast<T*>(glfwGetWindowUserPointer(mHandle)); }
        inline int getInputMode(int mode) const { return glfwGetInputMode(mHandle, mode); }
        inline void setInputMode(int mode, int value) const { glfwSetInputMode(mHandle, mode, value); }
        inline void getCursorPosition(double &x, double &y) const { return glfwGetCursorPos(mHandle, &x, &y); }
        inline void setCursorPosition(double x, double y) const { return glfwSetCursorPos(mHandle, x, y); }
        inline void makeContextCurrent() const { glfwMakeContextCurrent(mHandle); }
        inline void swapBuffers() const { glfwSwapBuffers(mHandle); }
        
        inline GLFWwindowposfun setPositionCallback(GLFWwindowposfun callback) const { return glfwSetWindowPosCallback(mHandle, callback); }
        inline GLFWwindowsizefun setSizeCallback(GLFWwindowsizefun callback) const { return glfwSetWindowSizeCallback(mHandle, callback); }
        inline GLFWwindowclosefun setCloseCallback(GLFWwindowclosefun callback) const { return glfwSetWindowCloseCallback(mHandle, callback); }
        inline GLFWwindowrefreshfun setRefreshCallback(GLFWwindowrefreshfun callback) const { return glfwSetWindowRefreshCallback(mHandle, callback); }
        inline GLFWwindowfocusfun setFocusCallback(GLFWwindowfocusfun callback) const { return glfwSetWindowFocusCallback(mHandle, callback); }
        inline GLFWwindowiconifyfun	setIconfigyCallback(GLFWwindowiconifyfun callback) const { return glfwSetWindowIconifyCallback(mHandle, callback); }
        inline GLFWframebuffersizefun setFramebufferSizeCallback(GLFWframebuffersizefun callback) const { return glfwSetFramebufferSizeCallback(mHandle, callback); }    
        inline GLFWkeyfun setKeyCallback(GLFWkeyfun callback) const { return glfwSetKeyCallback(mHandle, callback); }
        inline GLFWcharfun setCharCallback(GLFWcharfun callback) const { return glfwSetCharCallback(mHandle, callback); }
        inline GLFWcharmodsfun setCharModsCallback(GLFWcharmodsfun callback) const { return glfwSetCharModsCallback(mHandle, callback); }
        inline GLFWmousebuttonfun setMouseButtonCallback(GLFWmousebuttonfun callback) const { return glfwSetMouseButtonCallback(mHandle, callback); }
        inline GLFWcursorposfun setCursorPosCallback(GLFWcursorposfun callback) const { return glfwSetCursorPosCallback(mHandle, callback); }
        inline GLFWcursorenterfun setCursorEnterCallback(GLFWcursorenterfun callback) const { return glfwSetCursorEnterCallback(mHandle, callback); }
        inline GLFWscrollfun setScrollCallback(GLFWscrollfun callback) const { return glfwSetScrollCallback(mHandle, callback); }
        inline GLFWdropfun setDropCallback(GLFWdropfun callback ) const { return glfwSetDropCallback(mHandle, callback); }
        inline GLFWwindow *handle() const { return mHandle; }

        static inline void defaultHint() { GLFW::getInstance().defaultWindowHint(); }
        static inline void hint(int hint, int value) { GLFW::getInstance().windowHint(hint, value); }
        static inline Window getCurrentContext() { return Window(GLFW::getInstance().getCurrentContext(), false); }
        static Window &self(GLFWwindow *glfwWindow) { return *reinterpret_cast<ray::platform::Window *>(glfwGetWindowUserPointer(glfwWindow)); };                

        inline bool  isVisible() const { return getAttribute(GLFW_VISIBLE); }
        inline bool  hasFocus() const { return getAttribute(GLFW_FOCUSED); }
        inline int   width() const { int width, height; getSize(width, height); return width; }
        inline int   height() const { int width, height; getSize(width, height); return height; }
        inline float aspectRatio() const  { int width, height; getSize(width, height); return ((GLfloat)width) / ((GLfloat)height); }
        inline bool  isKeyPressed(const Key &key) const { return mIsKeyPressed[static_cast<int>(key)]; }
        inline bool  isKeyReleased(const Key &key) const { return mIsKeyReleased[static_cast<int>(key)]; }
        inline bool  isKeyHeld(const Key &key) const { return mIsKeyHeld[static_cast<int>(key)]; }
        inline bool  isMouseButtonPressed(const MouseButton &button) const { return mIsMouseButtonPressed[static_cast<int>(button)]; }
        inline bool  isMouseButtonReleased(const MouseButton &button) const { return mIsMouseButtonReleased[static_cast<int>(button)]; }
        inline bool  isMouseButtonHeld(const MouseButton &button) const { return mIsMouseButtonHeld[static_cast<int>(button)]; }
        inline void  showMouseCursor() const { setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
        inline void  hideMouseCursor() const { setInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }
        inline void  disableMouseCursor() const { setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

        void pollEvents()
        {
            mIsKeyPressed.reset();
            mIsKeyReleased.reset();
            mIsMouseButtonPressed.reset();
            mIsMouseButtonReleased.reset();
            GLFW::getInstance().pollEvents();
        }
    private:
        std::bitset<static_cast<size_t>(Key::KEY_LAST)> mIsKeyPressed, mIsKeyReleased, mIsKeyHeld;
        std::bitset<static_cast<size_t>(MouseButton::BUTTON_LAST)> mIsMouseButtonPressed, mIsMouseButtonReleased, mIsMouseButtonHeld;
    };

}}
