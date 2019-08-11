#pragma once

#include "../CameraBase.h"

class CCamera {
public:
	CCamera();										// Constructor - sets default values for camera position, viewvector, upvector, and speed
	~CCamera();										// Destructor

    void Create(
                const glm::vec3 &position = glm::vec3( 0.0f, 0.0f, -20.0f ),
                const glm::vec3 &front = glm::vec3( 0.0f, 0.0f, -1.0f ),
                const glm::vec3 &up = glm::vec3( 0.0f, 1.0f, 0.0f ),
                const glm::vec3 &worldUp = glm::vec3( 0.0f, 1.0f, 0.0f ),
                const GLfloat &fieldOfView = FOV,
                const GLfloat &width = (float)SCREEN_WIDTH,
                const GLfloat &height = (float)SCREEN_HEIGHT,
                const GLfloat &zNear = ZNEAR,
                const GLfloat &zFar = ZFAR);
    
    glm::vec3 GetPosition() const;                    // Gets the position of the camera centre of projection
    glm::vec3 GetView() const;                        // Gets the position of the camera view point
    glm::vec3 GetStrafeVector() const;                // Gets the camera strafe vector
    glm::mat4 GetViewProjection() const;
	glm::mat4* GetPerspectiveProjectionMatrix();	    // Gets the camera perspective projection matrix
	glm::mat4* GetOrthographicProjectionMatrix();	    // Gets the camera orthographic projection matrix
	glm::mat4 GetViewMatrix() const;				    // Gets the camera view matrix - note this is not stored in the class but returned using
    glm::mat4 GetModelMatrix() const;                   // Gets the camera model matrix
    glm::mat4 GetViewWithoutTranslation() const;        // Get view and removing translation properties from view matrix
    GLfloat GetFarPlane();                              // Gets far clipping plane
    GLfloat GetNearPlane();                             // Gets near clipping plane
    GLfloat GetFieldOfView();                           // Gets camera field of view
    GLfloat GetAspectRatio();                           // Gets camera aspect ratio
    glm::vec3 GetLeft();                               // Gets the camera left vector
    glm::vec3 GetRight();                              // Gets the camera right vector
    glm::vec3 GetUp();                                 // Gets the camera up vector
    glm::vec3 GetDown();                               // Gets the camera down vector
    glm::vec3 GetForward();                            // Gets the camera forward vector
    glm::vec3 GetBackward();                           // Gets the camera backward vector
    glm::vec3 GetVelocity();                           // Gets the camera velocity vector
    glm::mat4 GetPreviousMVP() const;                  // Gets the camera previous frame model view projection matrix
    GLboolean IsMoving() const;                         // Gets the camera movement
    
    // re-computer all the vector 
    void UpdateCameraVectors( );
    
	// Set the camera position, viewpoint, and up vector
	void Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector);
    
	// Rotate the camera viewpoint -- this effectively rotates the camera
	void RotateViewPoint(float angle, glm::vec3 &viewPoint);
    
    // Rotate the camera around a viewpoint
    void RotateAroundPoint(const float &distance, const glm::vec3 &viewpoint, const float &angle, const float &y);
    
    glm::vec3 PositionInFrontOfCamera( const GLfloat &distance);
    
	// Respond to mouse movement to rotate the camera
    void SetViewByMouse(const MouseState &state);

    // Respond to keyboard presses on arrow keys to translate the camera
    void TranslateByKeyboard(const double &dt, const KeyboardState &keyboardState);
    
	// Strafe the camera (move it side to side)
	void Strafe(double direction);

	// Advance the camera (move it forward or backward)
	void Advance(double direction);
    
    // Update the camera
    void Update(GLFWwindow *window, const GLdouble &dt,
                const MouseState &mouseState, const KeyboardState &keyboardState,
                const GLboolean &moveCamera);
    void Reset(GLFWwindow *window, const GLdouble &dt);
    
    // Set the projection matrices
    void SetPerspectiveProjectionMatrix(const GLfloat &fieldOfView, const GLfloat &aspectRatio, const GLfloat &nearClippingPlane, const GLfloat &farClippingPlane);
    void SetOrthographicProjectionMatrix(const GLfloat &width, const GLfloat height, const GLfloat &zNear, const GLfloat &zFar);
    void SetOrthographicProjectionMatrix(float value , float zNear, float zFar);
    void SetOrthographicProjectionMatrix(int width, int height);
    void SetModelMatrix(const glm::mat4 &model);
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void SetFieldOfView(const GLfloat &yoffset);
    // Set the camera velocity
    void SetVelocity(int deltaTime);
    
    glm::mat3 ComputeNormalMatrix(const glm::mat4 &modelMatrix);
    
    void Release();
    
private:

    //view and projection matrix
    glm::mat4 m_perspectiveProjectionMatrix;        // Perspective projection matrix
    glm::mat4 m_orthographicProjectionMatrix;        // Orthographic projection matrix
    glm::mat4 m_viewMatrix, m_modelMatrix;

    // movement
    GLboolean m_isMoving;
    
    // Camera Attributes
    glm::vec3 m_position;            // The position of the camera's centre of projection
    glm::vec3 m_previousPosition;    // The previous position of the camera's centre of projection
    glm::vec3 m_view;                // The camera's viewpoint (point where the camera is looking)
   
    glm::vec3 m_strafeVector;        // The camera's strafe vector
    
    glm::vec3 m_front;               // The camera's forward vector
    glm::vec3 m_back;                // The camera's backward vector
    glm::vec3 m_left;                // The camera's left vector
    glm::vec3 m_right;               // The camera's right vector
    glm::vec3 m_up;                  // The camera's up vector
    glm::vec3 m_down;                // The camera's down vector
    
    glm::vec3 m_worldUp;;            // The worlds up vector, the original position of the world
    
    // Eular Angles
    GLfloat m_fieldOfView;           // The view from the camera
    GLfloat m_zNear;                // The afr clipping plane
    GLfloat m_zFar;                   // The near clipping plane
    GLfloat m_yaw;                   // also known as Heading: rotation about y axis (aka “yaw”)
    GLfloat m_pitch;                 // rotation about x axis
    GLfloat m_roll;                  // also known as Bank: rotation about z axis (aka “roll”)
    GLfloat m_horizontalAngle;       // horizontal angle : toward -Z
    GLfloat m_verticalAngle;         // vertical angle : 0, toward -Y
    glm::vec3 m_velocity;            // The camera's velocity vector
    
    // Camera options
    GLfloat m_movementSpeed;         // How fast the camera moves
    GLfloat m_mouseSensitivity;      // How sensitive mouse is 
    
    // Screen 
    GLfloat m_screenWidth;           // the width size of the screen window
    GLfloat m_screenHeight;          // the height size of the screen window
    
    // Matrices
    glm::mat4 m_prevMVP;            // previous model->view->projection of the camera
    
};
