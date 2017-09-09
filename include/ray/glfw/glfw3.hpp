#pragma once

#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_GLCOREARB
#undef GLFW_INCLUDE_GLEXT
#include <string>
#include <vector>

namespace ray { namespace glfw {

    class Version 
    {
        friend class Library;
        int mMajor, mMinor, mRevision;
    public:
        Version() = default;
        Version(const Version &other) = default;
        Version(Version &&other) = default;
        inline int major() const { return mMajor; }
        inline int minor() const { return mMinor; }
        inline int revision() const { return mRevision; }
    };

    class VideoMode
    {
        friend class Monitor;
        const GLFWvidmode *mHandle;
        VideoMode(const GLFWvidmode *handle) : mHandle(handle) {}
    public:
        VideoMode(const VideoMode &other) = delete;
        VideoMode(VideoMode &&other) { mHandle = other.mHandle; other.mHandle = nullptr; }
        inline int width() const { return mHandle->width; }
        inline int height() const { return mHandle->height; }
        inline int redBits() const { return mHandle->redBits; }
        inline int greenBits() const { return mHandle->greenBits; }
        inline int blueBits() const { return mHandle->blueBits; }
        inline int refreshRate() const { return mHandle->refreshRate; }
    };

    class Monitor
    {
        friend class Library;
        friend class Window;
        GLFWmonitor *mHandle;
        Monitor(GLFWmonitor *handle) : mHandle(handle) { }
    public:
        Monitor(const Monitor &other) = delete;
        Monitor(Monitor &&other) { mHandle = other.mHandle; other.mHandle = nullptr; }
        inline void getPosition(int &x, int &y) const { glfwGetMonitorPos(mHandle, &x, &y); }
        inline void getPhysicalSize(int &x, int &y) const { glfwGetMonitorPhysicalSize(mHandle, &x, &y); }
        inline std::string getName() const { return glfwGetMonitorName(mHandle); }
        inline VideoMode getVideoMode() const { return VideoMode(glfwGetVideoMode(mHandle)); }
        inline std::vector<VideoMode> getVideoModes() const { 
            std::vector<VideoMode> result;
            int count = 0;
            auto allVideoMode = glfwGetVideoModes(mHandle, &count);
            for (int i = 0; i < count; ++i) result.push_back(VideoMode(&allVideoMode[i]));
            return result;
        }
        inline void setGamma(float gamma) const { glfwSetGamma(mHandle, gamma); }
        inline const GLFWgammaramp &getGammaRamp() const { return *glfwGetGammaRamp(mHandle); } // @TODO: create proper GammaRamp class
        inline void setGammaRamp(const GLFWgammaramp &ramp) const { glfwSetGammaRamp(mHandle, &ramp); } // @TODO: create proper GammaRamp class
        static inline Monitor getPrimary();
        static inline std::vector<Monitor> getInstances();
        static inline GLFWmonitorfun setCallback(GLFWmonitorfun callback);
    };

    class Cursor
    {
        friend class Library;
        friend class Window;
        GLFWcursor *mHandle;
        inline Cursor(GLFWcursor *handle) : mHandle(handle) {}
    public:
        inline Cursor(int shape);
        inline Cursor(const GLFWimage &image, int xhot, int yhot);
        inline Cursor(const Cursor &other) = delete;
        inline Cursor(Cursor &&other) { mHandle = other.mHandle; other.mHandle = nullptr; }
        inline ~Cursor() { glfwDestroyCursor(mHandle); }
    };

    class Window 
    {    
        friend class Library;
        GLFWwindow *mHandle;
        bool mShouldBeDestroyed;
        inline Window(GLFWwindow *handle, bool shouldBeDestroyed) : mHandle(handle), mShouldBeDestroyed(shouldBeDestroyed) { (void)mHandle; }
    public:
        inline Window(int width, int height, const std::string &title, const Monitor &monitor, const Window &share);
        inline Window(int width, int height, const std::string &title, const Window &share);
        inline Window(int width, int height, const std::string &title, const Monitor &monitor);
        inline Window(int width, int height, const std::string &title);
        inline Window(const Window &other) = delete;
        inline Window(Window &&other) { mHandle = other.mHandle; other.mHandle = nullptr; }
        inline virtual ~Window() { if (mShouldBeDestroyed) glfwDestroyWindow(mHandle); }
        inline bool shouldClose() const { return glfwWindowShouldClose(mHandle); }        
        inline void setShouldClose(bool value) const { return glfwSetWindowShouldClose(mHandle, value); }
        inline void setTitle(const std::string &title) const { glfwSetWindowTitle(mHandle, title.c_str()); }
        inline void setIcon(const std::vector<const GLFWimage> &images) const { glfwSetWindowIcon(mHandle, images.size(), &images[0]); } // @TODO: create proper Image class
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
        inline Monitor getMonitor() const { return Monitor(glfwGetWindowMonitor(mHandle)); }
        inline void setMonitor(const Monitor &monitor, int x, int y, int w, int h, int refreshRate) const { glfwSetWindowMonitor(mHandle, monitor.mHandle, x, y, w, h, refreshRate); }
        inline void setFullScreen(const Monitor &monitor, int w, int h, int refreshRate) const { glfwSetWindowMonitor(mHandle, monitor.mHandle, 0, 0, w, h, refreshRate); }
        inline void setWindowed(int x, int y, int w, int h) const { glfwSetWindowMonitor(mHandle, nullptr, x, y, w, h, 0); }
        inline int getAttribute(int attribute) const { return glfwGetWindowAttrib(mHandle, attribute); }
        inline void setUserPointer(void *userPointer) const { glfwSetWindowUserPointer(mHandle, userPointer); }
        inline void *getUserPointer() const { return glfwGetWindowUserPointer(mHandle); }
        template<typename T> inline void setUserPointer(T &userPointer) const { glfwSetWindowUserPointer(mHandle, reinterpret_cast<void*>(&userPointer)); }
        template<typename T> inline T &getUserPointer() const { return *reinterpret_cast<T*>(glfwGetWindowUserPointer(mHandle)); }
        inline GLFWwindowposfun setPositionCallback(GLFWwindowposfun callback) const { return glfwSetWindowPosCallback(mHandle, callback); }
        inline GLFWwindowsizefun setSizeCallback(GLFWwindowsizefun callback) const { return glfwSetWindowSizeCallback(mHandle, callback); }
        inline GLFWwindowclosefun setCloseCallback(GLFWwindowclosefun callback) const { return glfwSetWindowCloseCallback(mHandle, callback); }
        inline GLFWwindowrefreshfun setRefreshCallback(GLFWwindowrefreshfun callback) const { return glfwSetWindowRefreshCallback(mHandle, callback); }
        inline GLFWwindowfocusfun setFocusCallback(GLFWwindowfocusfun callback) const { return glfwSetWindowFocusCallback(mHandle, callback); }
        inline GLFWwindowiconifyfun	setIconfigyCallback(GLFWwindowiconifyfun callback) const { return glfwSetWindowIconifyCallback(mHandle, callback); }
        inline GLFWframebuffersizefun setFramebufferSizeCallback(GLFWframebuffersizefun callback) const { return glfwSetFramebufferSizeCallback(mHandle, callback); }    
        inline int getInputMode(int mode) const { return glfwGetInputMode(mHandle, mode); }
        inline void setInputMode(int mode, int value) const { glfwSetInputMode(mHandle, mode, value); }
        inline std::string getKeyName(int key, int scancode) const { return glfwGetKeyName(key, scancode); }
        inline int getKey(int key) const { return glfwGetKey(mHandle, key); }
        inline int getMouseButton(int button) const { return glfwGetMouseButton(mHandle, button); }
        inline void getCursorPosition(double &x, double &y) const { return glfwGetCursorPos(mHandle, &x, &y); }
        inline void setCursorPosition(double x, double y) const { return glfwSetCursorPos(mHandle, x, y); }
        inline void setCursor(const Cursor &cursor) const { glfwSetCursor(mHandle, cursor.mHandle); }
        inline void makeContextCurrent() const { glfwMakeContextCurrent(mHandle); }
        inline void swapBuffers() const { glfwSwapBuffers(mHandle); }
        inline std::string getClipboardString() const { return glfwGetClipboardString(mHandle); }
        inline void setClipboardString(const std::string &string) const { glfwSetClipboardString(mHandle, string.c_str()); }
        inline GLFWkeyfun setKeyCallback(GLFWkeyfun callback) const { return glfwSetKeyCallback(mHandle, callback); }
        inline GLFWcharfun setCharCallback(GLFWcharfun callback) const { return glfwSetCharCallback(mHandle, callback); }
        inline GLFWcharmodsfun setCharModsCallback(GLFWcharmodsfun callback) const { return glfwSetCharModsCallback(mHandle, callback); }
        inline GLFWmousebuttonfun setMouseButtonCallback(GLFWmousebuttonfun callback) const { return glfwSetMouseButtonCallback(mHandle, callback); }
        inline GLFWcursorposfun setCursorPosCallback(GLFWcursorposfun callback) const { return glfwSetCursorPosCallback(mHandle, callback); }
        inline GLFWcursorenterfun setCursorEnterCallback(GLFWcursorenterfun callback) const { return glfwSetCursorEnterCallback(mHandle, callback); }
        inline GLFWscrollfun setScrollbackCallback(GLFWscrollfun callback) const { return glfwSetScrollCallback(mHandle, callback); }
        inline GLFWdropfun setDropCallback(GLFWdropfun callback ) const { return glfwSetDropCallback(mHandle, callback); }
        inline GLFWwindow *handle() const { return mHandle; }

        static inline void defaultHint();
        static inline void hint(int hint, int value);
        static inline void pollEvents();
        static inline void waitEvents(); 
        static inline void waitEventsTimeout(double timeout); 
        static inline void postEmptyEvent(); 
        static Window getCurrentContext();
    };

    class Joystick
    {
        friend class Library;
        int mHandle;
        Joystick(int handle) : mHandle(handle) {}
    public:
        Joystick(const Joystick &other) = default;
        Joystick(Joystick &&other) = default;
        virtual ~Joystick() = default;

        inline bool isPresent() const { return glfwJoystickPresent(mHandle); }
        inline std::vector<float> getAxes() const { int count; auto axes = glfwGetJoystickAxes(mHandle, &count); return std::vector<float>(*axes, count); }
        inline std::vector<uint8_t> getButtons() const { int count; auto buttons = glfwGetJoystickButtons(mHandle, &count ); return std::vector<uint8_t>(*buttons, count); }
        inline std::string getName() const { return glfwGetJoystickName(mHandle); }

        static inline GLFWjoystickfun setJoystickCallback(GLFWjoystickfun cbfun) { return glfwSetJoystickCallback(cbfun); }
    };

    class Library 
    {
        friend class Window;
        friend class Monitor;    
        friend class Cursor;
        inline Library() 
        { 
            glfwInit();
            windowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            windowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            windowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            windowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            
        }
    public:
        static inline const Library &getInstance() { static Library instance;return instance; }
        inline ~Library() { glfwTerminate(); }
        inline Library(const Library &other) = delete; 
        inline Library(Library &other) = delete;
        inline void getVersion(int &major, int &minor, int &rev) const { glfwGetVersion(&major, &minor, &rev); }
        inline Version getVersion() const { Version version; getVersion(version.mMajor, version.mMinor, version.mRevision); return version; }
        inline std::string  getVersionString() const { return glfwGetVersionString(); }
        inline GLFWerrorfun setErrorCallback(GLFWerrorfun callback) const { return glfwSetErrorCallback(callback); } 
        inline double getTime(void) const { return glfwGetTime(); }
        inline void setTime(double time) { return glfwSetTime(time); }
        inline uint64_t getTimerValue() const { return glfwGetTimerValue(); }
        inline uint64_t getTimerFrequency() const { return glfwGetTimerFrequency(); }
        inline void swapInterval(int interval) const { return glfwSwapInterval(interval); }
        inline int extensionSupported(const std::string &extension) const { return glfwExtensionSupported(extension.c_str()); }
        inline GLFWglproc getProcAddress(const std::string &proc) const {  return glfwGetProcAddress(proc.c_str()); }

    private:
        inline GLFWmonitor *getPrimaryMonitor() const { return glfwGetPrimaryMonitor(); }
        inline GLFWmonitor **getMonitors(int &count) const { return glfwGetMonitors(&count); }
        inline GLFWmonitorfun setMonitorCallback(GLFWmonitorfun callback) const { return glfwSetMonitorCallback(callback); }
        inline void defaultWindowHint() const { glfwDefaultWindowHints(); }
        inline void windowHint(int hint, int value) const { glfwWindowHint(hint, value); }
        inline GLFWwindow *createWindow(int width, int height, std::string title, GLFWmonitor *monitor, GLFWwindow *share) const { return glfwCreateWindow(width, height, title.c_str(), monitor, share); }
        inline GLFWwindow *getCurrentContext() const { return glfwGetCurrentContext(); }

        inline void pollEvents() const { glfwPollEvents(); }
        inline void waitEvents() const { glfwWaitEvents(); }
        inline void waitEventsTimeout(double timeout) const { glfwWaitEventsTimeout(timeout); }
        inline void postEmptyEvent() const { glfwPostEmptyEvent(); }
        inline GLFWcursor *createCursor(const GLFWimage &image, int xhot, int yhot) const { return glfwCreateCursor(&image, xhot, yhot); }
        inline GLFWcursor *createStandardCursor(int shape) const { return glfwCreateStandardCursor(shape); }
    };

    inline Monitor Monitor::getPrimary() { return Library::getInstance().getPrimaryMonitor(); }
    inline std::vector<Monitor> Monitor::getInstances() 
    {            
            int count = 0;
            std::vector<Monitor> result;
            auto monitors = Library::getInstance().getMonitors(count);
            for (int i = 0; i < count; ++i) result.push_back(Monitor(monitors[i]));
            return result;
    }
    inline GLFWmonitorfun Monitor::setCallback(GLFWmonitorfun callback) { return Library::getInstance().setMonitorCallback(callback); }

    inline Window::Window(int width, int height, const std::string &title, const Monitor &monitor, const Window &share) : mHandle(Library::getInstance().createWindow(width, height, title, monitor.mHandle, share.mHandle)), mShouldBeDestroyed(true) {}
    inline Window::Window(int width, int height, const std::string &title, const Window &share) : mHandle(Library::getInstance().createWindow(width, height, title, nullptr, share.mHandle)), mShouldBeDestroyed(true) {}
    inline Window::Window(int width, int height, const std::string &title, const Monitor &monitor) : mHandle(Library::getInstance().createWindow(width, height, title, monitor.mHandle, nullptr)), mShouldBeDestroyed(true) {}
    inline Window::Window(int width, int height, const std::string &title) : mHandle(Library::getInstance().createWindow(width, height, title, nullptr, nullptr)), mShouldBeDestroyed(true) {}
    inline void Window::defaultHint() { Library::getInstance().defaultWindowHint(); }
    inline void Window::hint(int hint, int value) { Library::getInstance().windowHint(hint, value); }
    inline void Window::pollEvents() { Library::getInstance().pollEvents(); }
    inline void Window::waitEvents() { Library::getInstance().waitEvents(); }
    inline void Window::waitEventsTimeout(double timeout) { Library::getInstance().waitEventsTimeout(timeout); }
    inline void Window::postEmptyEvent() { Library::getInstance().postEmptyEvent(); }
    inline Window Window::getCurrentContext() { return Window(Library::getInstance().getCurrentContext(), false); }

    inline Cursor::Cursor(const GLFWimage &image, int xhot, int yhot) : mHandle(Library::getInstance().createCursor(image, xhot, yhot)) { }
    inline Cursor::Cursor(int shape) : mHandle(Library::getInstance().createStandardCursor(shape)) {}
}}
