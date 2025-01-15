#include <GLFW/glfw3.h>
#include <engine/camera.hpp> // Sua classe Camera
#include <engine/AudioManeger.hpp> // Sua classe SoundManager

class InputHandler {
private:
    GLFWwindow* window;
    Camera& camera;
    AudioManager& sound;
    double lastPlayTime;
    const double delay = 0.2; // Delay em segundos para os sons repetidos

public:
    InputHandler(GLFWwindow* window, Camera& camera, AudioManager& sound)
        : window(window), camera(camera), sound(sound), lastPlayTime(0.0) {}

    void processInput(float deltaTime) {
        int esc = glfwGetKey(window, GLFW_KEY_ESCAPE);
        int w = glfwGetKey(window, GLFW_KEY_W);
        int s = glfwGetKey(window, GLFW_KEY_S);
        int a = glfwGetKey(window, GLFW_KEY_A);
        int d = glfwGetKey(window, GLFW_KEY_D);
        int shiftL = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        int shiftR = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);

        bool isWalking = w == GLFW_PRESS || a == GLFW_PRESS || s == GLFW_PRESS || d == GLFW_PRESS;
        bool isRunning = isWalking && (shiftL == GLFW_PRESS || shiftR == GLFW_PRESS);

        if (esc == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        double currentTime = glfwGetTime();
        float speed = isRunning ? 1.2f : 0.5f;

        if (w == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime * speed, isRunning);
        if (s == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime * speed, isRunning);
        if (a == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime * speed, isRunning);
        if (d == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime * speed, isRunning);

        if (!isWalking) {
            camera.ProcessKeyboard(NONE, deltaTime * speed, false);
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            sound.playAudio(0);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (currentTime - lastPlayTime >= delay) {
                sound.playAudioRepeter(2);
                lastPlayTime = currentTime;
            }
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            if (currentTime - lastPlayTime >= delay) {
                sound.playAudioRepeter(1);
                lastPlayTime = currentTime;
            }
        }
    }
};
