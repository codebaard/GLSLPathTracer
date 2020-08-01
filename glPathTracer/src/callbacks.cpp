#include <callbacks.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    //jLog::Instance()->Log(INFO, "Mouse Callback called.");

    if (glfwHandler::giveCamera()->firstMouse)
    {
        glfwHandler::giveCamera()->lastX = xpos;
        glfwHandler::giveCamera()->lastY = ypos;
        glfwHandler::giveCamera()->firstMouse = false;
    }

    float xoffset = xpos - glfwHandler::giveCamera()->lastX;
    float yoffset = glfwHandler::giveCamera()->lastY - ypos; // reversed since y-coordinates go from bottom to top

    glfwHandler::giveCamera()->lastX = xpos;
    glfwHandler::giveCamera()->lastY = ypos;

    glfwHandler::giveCamera()->camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	glfwHandler::giveCamera()->camera.ProcessMouseScroll(yoffset);
}

