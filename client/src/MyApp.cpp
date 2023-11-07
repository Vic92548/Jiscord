#include "MyApp.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif


#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 400

int sock = -1;

MyApp::MyApp() {
  ///
  /// Create our main App instance.
  ///
  app_ = App::Create();

  ///
  /// Create a resizable window by passing by OR'ing our window flags with
  /// kWindowFlags_Resizable.
  ///
  window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
    false, kWindowFlags_Titled | kWindowFlags_Resizable);


#ifdef _WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if (result != 0) {
        // Handle error
    }
#endif


    ///
  /// Create our HTML overlay-- we don't care about its initial size and
  /// position because it'll be calculated when we call OnResize() below.
  ///
  overlay_ = Overlay::Create(window_, 1, 1, 0, 0);

  ///
  /// Force a call to OnResize to perform size/layout of our overlay.
  ///
  OnResize(window_.get(), window_->width(), window_->height());

  ///
  /// Load a page into our overlay's View
  ///
  overlay_->view()->LoadURL("file:///app.html");

  ///
  /// Register our MyApp instance as an AppListener so we can handle the
  /// App's OnUpdate event below.
  ///
  app_->set_listener(this);

  ///
  /// Register our MyApp instance as a WindowListener so we can handle the
  /// Window's OnResize event below.
  ///
  window_->set_listener(this);

  ///
  /// Register our MyApp instance as a LoadListener so we can handle the
  /// View's OnFinishLoading and OnDOMReady events below.
  ///
  overlay_->view()->set_load_listener(this);

  ///
  /// Register our MyApp instance as a ViewListener so we can handle the
  /// View's OnChangeCursor and OnChangeTitle events below.
  ///
  overlay_->view()->set_view_listener(this);
}

// This callback will be bound to 'OnButtonClick()' on the page.
JSValueRef formSubmit(JSContextRef ctx, JSObjectRef function,
                         JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef arguments[], JSValueRef* exception) {

    if (argumentCount < 2) {
        // Handle error - not enough arguments
        // You could set the exception here if needed
        return JSValueMakeUndefined(ctx);
    }

    // Extract the arguments from JavaScript
    JSStringRef hostnameJS = JSValueToStringCopy(ctx, arguments[0], exception);
    size_t hostnameSize = JSStringGetMaximumUTF8CStringSize(hostnameJS);
    char* hostname = new char[hostnameSize];
    JSStringGetUTF8CString(hostnameJS, hostname, hostnameSize);

    JSStringRef portJS = JSValueToStringCopy(ctx, arguments[1], exception);
    size_t portSize = JSStringGetMaximumUTF8CStringSize(portJS);
    char* portStr = new char[portSize];
    JSStringGetUTF8CString(portJS, portStr, portSize);
    int port = atoi(portStr);


#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
        // Handle error - unable to initialize Winsock
    }
#endif

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        // Handle error - unable to create socket
#ifdef _WIN32
        WSACleanup();
#endif
    }

    // Define server address
    const int serverPort = port;       // Replace with your server's port number

    struct sockaddr_in server_addr;
    struct hostent *server;

    // Convert hostname to IP address
    server = gethostbyname(hostname);
    if (server == NULL) {
        // Handle error - the host could not be resolved
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
    }

    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(serverPort);

    // Attempt to connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        // Handle error - unable to connect
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        // Execute JavaScript to indicate connection failure
        JSStringRef failureScript = JSStringCreateWithUTF8CString(
                "document.querySelector('#connection-dialog dialog').lastElementChild.innerText = 'Connection failed'"
        );
        JSEvaluateScript(ctx, failureScript, 0, 0, 0, 0);
        JSStringRelease(failureScript);

        JSStringRelease(hostnameJS);
        delete[] hostname;
        JSStringRelease(portJS);
        delete[] portStr;

        return JSValueMakeNull(ctx);
    }

    // Execute JavaScript to indicate connection success
    JSStringRef successScript = JSStringCreateWithUTF8CString(
            "document.querySelector('#connection-dialog').style.display = 'none'"
    );
    JSEvaluateScript(ctx, successScript, 0, 0, 0, 0);
    JSStringRelease(successScript);

    JSStringRelease(hostnameJS);
    delete[] hostname;
    JSStringRelease(portJS);
    delete[] portStr;

    return JSValueMakeNull(ctx);
}


MyApp::~MyApp() {
    if (sock != -1) {
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
    }
}

void MyApp::Run() {
  app_->Run();
}

void MyApp::OnUpdate() {
  ///
  /// This is called repeatedly from the application's update loop.
  ///
  /// You should update any app logic here.
  ///
}

void MyApp::OnClose(ultralight::Window* window) {
  app_->Quit();
}

void MyApp::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
  ///
  /// This is called whenever the window changes size (values in pixels).
  ///
  /// We resize our overlay here to take up the entire window.
  ///
  overlay_->Resize(width, height);
}

void MyApp::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

JSValueRef sendMessage(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                              const JSValueRef *arguments, JSValueRef *exception) {
    // Make sure there is at least one argument (the message to send)
    if (argumentCount < 1) {
        // Handle the error, possibly by setting an exception
        // and returning undefined
        return JSValueMakeUndefined(ctx);
    }

    // Convert the JSValueRef argument to a string
    JSStringRef messageJS = JSValueToStringCopy(ctx, arguments[0], exception);
    size_t messageSize = JSStringGetMaximumUTF8CStringSize(messageJS);
    char* message = new char[messageSize];
    JSStringGetUTF8CString(messageJS, message, messageSize);

    // Send the message to the server using the socket
    // Make sure the socket is created and connected before sending
    if (sock >= 0) {
        int bytes_sent = send(sock, message, strlen(message), 0);

        // Check if sending succeeded
        if (bytes_sent < 0) {
            JSStringRelease(messageJS);
            delete[] message;
            return JSValueMakeUndefined(ctx);
        }
    } else {
        // Handle the error, the socket is not connected
        // ...
    }

    // Clean up the memory allocated for the message
    JSStringRelease(messageJS);
    delete[] message;

    // Return null to indicate that the function doesn't return anything
    return JSValueMakeNull(ctx);

}


JSValueRef receiveMessage(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                          const JSValueRef *arguments, JSValueRef *exception) {
    // No arguments are expected for this function
    char buffer[1024] = {0}; // Adjust buffer size as necessary

    // Check the socket for available data
    if (sock >= 0) {
        // Set up the file descriptor set for `select`
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        // Set timeout to 0 so select doesn't block
        timeval timeout;
        timeout.tv_sec = 0; // Zero seconds
        timeout.tv_usec = 0; // Zero milliseconds

        // Check if there is something to read
        int select_result = select(sock + 1, &readfds, NULL, NULL, &timeout);

        // Error handling for select
        if (select_result < 0) {
            // Handle select error
            // ...
            return JSValueMakeUndefined(ctx);
        } else if (select_result == 0) {
            // No data available to read
            return JSValueMakeNull(ctx);
        }

        // If select indicates that the socket is ready for reading
        int bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

        // Error handling for recv
        if (bytesRead < 0) {
            // Handle recv error
            // ...
            return JSValueMakeUndefined(ctx);
        } else if (bytesRead == 0) {
            // No message was received
            return JSValueMakeNull(ctx);
        }

        // Otherwise, we have received data
        buffer[bytesRead] = '\0'; // Null-terminate the string
    } else {
        // Handle the error, the socket is not connected
        // ...
        return JSValueMakeUndefined(ctx);
    }

    // Create a JavaScript String from the received message
    JSStringRef messageJS = JSStringCreateWithUTF8CString(buffer);
    JSValueRef messageValue = JSValueMakeString(ctx, messageJS);
    JSStringRelease(messageJS);

    // Return the received message to JavaScript
    return messageValue;
}



void MyApp::OnDOMReady(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const String& url) {

    // Acquire the JS execution context for the current page.
    auto scoped_context = caller->LockJSContext();

    // Typecast to the underlying JSContextRef.
    JSContextRef ctx = (*scoped_context);

    // Create a JavaScript String containing the name of our callback.
    JSStringRef name = JSStringCreateWithUTF8CString("formSubmit");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
                                                        formSubmit);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(name);

    JSStringRef sendMessageName = JSStringCreateWithUTF8CString("sendMessage");
    JSObjectRef sendMessageFunc = JSObjectMakeFunctionWithCallback(ctx, sendMessageName, sendMessage);
    JSObjectSetProperty(ctx, globalObj, sendMessageName, sendMessageFunc, 0, 0);
    JSStringRelease(sendMessageName);

    JSStringRef receiveMessageName = JSStringCreateWithUTF8CString("receiveMessage");
    JSObjectRef receiveMessageFunc = JSObjectMakeFunctionWithCallback(ctx, receiveMessageName, receiveMessage);
    JSObjectSetProperty(ctx, globalObj, receiveMessageName, receiveMessageFunc, 0, 0);
    JSStringRelease(receiveMessageName);
}

void MyApp::OnChangeCursor(ultralight::View* caller,
                           Cursor cursor) {
  ///
  /// This is called whenever the page requests to change the cursor.
  ///
  /// We update the main window's cursor here.
  ///
  window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View* caller,
                          const String& title) {
  ///
  /// This is called whenever the page requests to change the title.
  ///
  /// We update the main window's title here.
  ///
  window_->SetTitle(title.utf8().data());
}

