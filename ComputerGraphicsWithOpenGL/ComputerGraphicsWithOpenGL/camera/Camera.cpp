#include "Camera.h"
// https://stackoverflow.com/questions/29451567/how-to-move-in-the-direction-of-the-camera-opengl-c
// https://gamedev.stackexchange.com/questions/136236/opengl-camera-movement-with-mouse-buttons
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/

// Constructor for camera -- initialise with some default values
CCamera::CCamera()
{
	m_position = glm::vec3(0.0f, 0.0f, -15.0f);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_view = glm::vec3(0.0f, 0.0f, 0.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_worldUp = glm::vec3( 0.0f, 1.0f, 0.0f );
    m_pitch = PITCH;
    m_yaw = YAW;
    m_fieldOfView = FOV;
    m_movementSpeed = (GLfloat)SPEED;
    m_mouseSensitivity = (GLfloat)SENSITIVTY;
    m_screenWidth = (GLfloat)SCREEN_WIDTH;
    m_screenHeight = (GLfloat)SCREEN_HEIGHT;
    SetPerspectiveProjectionMatrix(m_fieldOfView, (m_screenWidth/m_screenHeight), ZNEAR, ZFAR);
    SetOrthographicProjectionMatrix(m_screenWidth, m_screenHeight );
}
CCamera::~CCamera()
{
    Release();
}
 
void CCamera::Create(
            const glm::vec3 &position,
            const glm::vec3 &worldUp,
            const GLfloat &pitch,
            const GLfloat &yaw,
            const GLfloat &fieldOfView,
            const GLfloat &width,
            const GLfloat &height,
            const GLfloat &zNear,
            const GLfloat &zFar) {
    
    this->m_position = position;
    this->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->m_worldUp = worldUp;
    this->m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->m_fieldOfView = fieldOfView;
    this->SetPerspectiveProjectionMatrix(fieldOfView, (width/height), zNear, zFar);
    this->SetOrthographicProjectionMatrix(width, height);
    this->m_pitch = pitch;
    this->m_yaw = yaw;
    this->m_movementSpeed = (GLfloat)SPEED;
    this->m_mouseSensitivity = (GLfloat)SENSITIVTY;
    this->m_screenWidth = width;
    this->m_screenHeight = height;
    this->UpdateCameraVectors();
}


// Calculates the front vector from the Camera's (updated) Eular Angles
void CCamera::UpdateCameraVectors( )
{
   
    // Calculate the new Front vector using direction
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    //glm::vec3 direction;
    //direction.x = cos( glm::radians( this->m_yaw ) ) * cos( glm::radians( this->m_pitch ) );
    //direction.y = sin( glm::radians( this->m_pitch ) );
    //direction.z = sin( glm::radians( this->m_yaw ) ) * cos( glm::radians( this->m_pitch ) );
    
    //this->m_front = glm::normalize( direction );
    this->m_back = glm::normalize(m_front) * -1.0f;
    
    // Also re-calculate the Right and Up vector
    this->m_right = glm::normalize( glm::cross( this->m_front, this->m_worldUp ) );  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->m_left = glm::normalize(m_right) * -1.0f;
    
    // Up vector : perpendicular to both direction and right
    this->m_up = glm::normalize( glm::cross( this->m_right, this->m_front ) );
    this->m_down = glm::normalize(m_up) * -1.0f;
    
    this->m_view = m_position + m_front;
    this->m_viewMatrix = glm::lookAt(
                                     m_position, // what position you want the camera to be at when looking at something in World Space
                                     m_view, // // what position you want the camera to be  looking at in World Space, meaning look at what(using vec3) ?  // meaning the camera view point
                                     m_up  //which direction is up, you can set to (0,-1,0) to look upside-down
                                     );
    
   
}


// Set the camera at a specific position, looking at the view point, with a given up vector
void CCamera::Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector)
{
	m_position = position;
    m_front = glm::normalize(viewpoint - position); // finding front vector
	m_up = upVector;
    m_worldUp = glm::vec3( 0.0f, 1.0f, 0.0f );
    
    UpdateCameraVectors();
}


// Respond to mouse movement
void CCamera::SetViewByMouse(const GLfloat &mouseX, const GLfloat &mouseY, const GLboolean &constrainPitch, const bool &enableMouse)
{
    if (enableMouse) {
        GLfloat lastX = SCREEN_WIDTH / 2.0f;
        GLfloat lastY = SCREEN_HEIGHT / 2.0f;
        
        if( m_firstMouse )
        {
            lastX = mouseX;
            lastY = mouseY;
            m_firstMouse = false;
        }
        
        GLfloat xOffset = mouseX - lastX;
        GLfloat yOffset = lastY - mouseY;  // Reversed since y-coordinates go from bottom to left
        
        lastX = mouseX;
        lastY = mouseY;
        
        xOffset *= this->m_mouseSensitivity;
        yOffset *= this->m_mouseSensitivity;
        
        this->m_pitch += yOffset; // up down
        this->m_yaw   += xOffset; //left right
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if ( constrainPitch )
        {
            if ( this->m_pitch > 89.0f )
            {
                this->m_pitch = 89.0f;
            }
            
            if ( this->m_pitch < -89.0f )
            {
                this->m_pitch = -89.0f;
            }
        }
        
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->UpdateCameraVectors( );
    }
    
}

// Respond to mouse movement
void CCamera::SetViewByMouse(GLFWwindow *window, const bool &enableMouse)
{  
    
    if (enableMouse) {
        double middle_x = (int)m_screenWidth >> 1;
        double middle_y = (int)m_screenHeight >> 1;
        
        static float rotation_x = 0.0f;

        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        
        if (mouse_x == middle_x && mouse_y == middle_y) {
            return;
        }
        
        glfwSetCursorPos(window, (double)middle_x, (double)middle_y);
        
        m_verticalAngle = (float) (middle_x - mouse_x) / 1000.0f;
        m_horizontalAngle = (float) (middle_y - mouse_y) / 1000.0f;

        rotation_x -= m_horizontalAngle;

        float maxAngle = 1.56f; // Just a little bit below PI / 2

        if (rotation_x > maxAngle) {
            rotation_x = maxAngle;
        } else if (rotation_x < -maxAngle) {
            rotation_x = -maxAngle;
        } else {
            glm::vec3 cross = glm::cross(m_view - m_position, m_up);
            glm::vec3 axis = glm::normalize(cross);

            RotateViewPoint(m_horizontalAngle, axis);
        }
        
        glm::vec3 viewPoint = m_worldUp;
        RotateViewPoint(m_verticalAngle, viewPoint);
    
    }
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void CCamera::RotateViewPoint(float fAngle, glm::vec3 &vPoint)
{
	glm::vec3 vView = m_view - m_position;// direction vector
	
	glm::mat4 R = glm::rotate(glm::mat4(1), fAngle * 180.0f / (float) M_PI, vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);

    m_front = glm::normalize(glm::vec3(newView));
    //m_view = m_position + m_front;
    
    UpdateCameraVectors();
}

void CCamera::RotateAroundPoint(const float &distance, const glm::vec3 &viewpoint, const float &angle, const float &y){
    
    // https://stackoverflow.com/questions/19990146/orbiting-object-around-orbiting-object
    
    float radian = glm::radians(angle);
   
    float camX = viewpoint.x + (distance * cosf(radian));
    float camY = y;
    float camZ = viewpoint.z + (distance * sinf(radian));
    
    // Set the camera position and lookat point
    glm::vec3 position = glm::vec3(camX, camY, camZ);   // Camera position
    glm::vec3 look = viewpoint; // Look at point
    glm::vec3 upV = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector

    Set(position, look, upV);
    
}

glm::vec3 CCamera::PositionInFrontOfCamera( const GLfloat &distance){
    return GetPosition() + (GetForward() * distance); 
}

// Strafe the camera (side to side motion) (Left - Right Motion)
void CCamera::Strafe(double direction)
{
	float speed = (float) (SPEEDRATIO * direction);

	m_position.x = m_position.x + m_strafeVector.x * speed;
	m_position.z = m_position.z + m_strafeVector.z * speed;

	//m_view.x = m_view.x + m_strafeVector.x * speed;
	//m_view.z = m_view.z + m_strafeVector.z * speed;
    
    UpdateCameraVectors();
}

// Advance the camera (forward / backward motion)
void CCamera::Advance(double direction)
{
	float speed = (float) (SPEEDRATIO * direction);

	glm::vec3 view = glm::normalize(m_view - m_position);
	m_position = m_position + view * speed;
	//m_view = m_view + view * speed;
    
    UpdateCameraVectors();
}

// Set the camera velocity
void  CCamera::SetVelocity(int deltaTime) {
    glm::vec3 displacement = m_position - m_previousPosition;         // calculate the displacement
    GLfloat distance = glm::distance(m_previousPosition, m_position); // calculate the distance moved
    GLfloat timePerSecond = (float)(deltaTime / 1000.0f); // this is the elapsed time during a second
    GLfloat speed = distance / timePerSecond; // the speed is measured in metres per second (m/s) and not meters per frame.
    m_velocity = displacement * (speed / distance);                 // calculate the velocity vector
}


// Update the camera to respond to mouse motion for rotations and keyboard for translation
void CCamera::Update(GLFWwindow *window, const double &dt, const int &key, const bool &moveCamera, const bool &enableMouse)
{
    m_previousPosition = m_position;
	glm::vec3 vector = glm::cross(m_view - m_position, m_up);
	m_strafeVector = glm::normalize(vector);

    if (moveCamera){ 
        SetViewByMouse(window, enableMouse);
        TranslateByKeyboard(dt, key);
    }
    
    // Update the velocity of the camera
    SetVelocity(dt);
}

// Update the camera to respond to key presses for translation
void CCamera::TranslateByKeyboard(const double &dt, const int &keyPressed)
{
 
    if (keyPressed != -1){
        
        // FORWARD
        if (keyPressed == GLFW_KEY_UP || keyPressed == GLFW_KEY_W ) {
            Advance(m_movementSpeed * dt);
        }
        
        // BACKWARD
        if (keyPressed == GLFW_KEY_DOWN || keyPressed == GLFW_KEY_S ) {
            Advance(-m_movementSpeed * dt);
        }
        
        // LEFT
        if (keyPressed == GLFW_KEY_LEFT || keyPressed == GLFW_KEY_A ) {
            Strafe(-m_movementSpeed * dt);
        }
        
        //RIGHT
        if (keyPressed == GLFW_KEY_RIGHT || keyPressed == GLFW_KEY_D ) {
            Strafe(m_movementSpeed * dt);
        }
    }
    
    
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes

void CCamera::SetPerspectiveProjectionMatrix(const GLfloat &fieldOfView, const GLfloat &aspectRatio, const GLfloat &nearClippingPlane, const GLfloat &farClippingPlane){
    this->m_fieldOfView = fieldOfView;
    this->m_perspectiveProjectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void CCamera::SetOrthographicProjectionMatrix(const GLfloat &width, const GLfloat height, const GLfloat &zNear, const GLfloat &zFar){
    this->m_orthographicProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, zNear, zFar);
}

void CCamera::SetOrthographicProjectionMatrix(float value , float zNear, float zFar)
{
    m_orthographicProjectionMatrix = glm::ortho(-value, value, -value, value, zNear, zFar);
}

void CCamera::SetOrthographicProjectionMatrix(int width, int height)
{
    m_orthographicProjectionMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
}

// Return the camera position
glm::vec3 CCamera::GetPosition() const
{
    return m_position;
}

// Return the camera view point
glm::vec3 CCamera::GetView() const
{
    return m_view;
}

// Return the camera strafe vector
glm::vec3 CCamera::GetStrafeVector() const
{
    return m_strafeVector;
}

// Return the camera perspective projection matrix
glm::mat4* CCamera::GetPerspectiveProjectionMatrix()
{
    return &m_perspectiveProjectionMatrix;
}

// Return the camera orthographic projection matrix
glm::mat4* CCamera::GetOrthographicProjectionMatrix()
{
    return &m_orthographicProjectionMatrix;
}

// Get the camera view matrix
glm::mat4 CCamera::GetViewMatrix()
{
    return this->m_viewMatrix;
}

glm::mat4 CCamera::GetViewProjection() const { 
    return this->m_perspectiveProjectionMatrix * this->m_viewMatrix; 
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 CCamera::ComputeNormalMatrix(const glm::mat4 &modelMatrix)
{
	return glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}


// http://roy-t.nl/2010/03/04/getting-the-left-forward-and-back-vectors-from-a-view-matrix-directly.html
// Because a ViewMatrix is an inverse transposed matrix, viewMatrix.Left is not the real left
// These methods returns the real .Left, .Right, .Up, .Down, .Forward, .Backward
// See: http://forums.xna.com/forums/t/48799.aspx

// Return the camera left vector
glm::vec3 CCamera::GetLeft()
{
    return m_left;
}

// Return the camera right vector
glm::vec3 CCamera::GetRight()
{
    return m_right;
}

// Return the camera up vector
glm::vec3 CCamera::GetUp()
{
    return m_up;
}

// Return the camera down vector
glm::vec3 CCamera::GetDown()
{
    return m_down;
}

// Return the camera forward vector
glm::vec3 CCamera::GetForward()
{
    return m_front;
}

// Return the camera backward vector
glm::vec3 CCamera::GetBackward()
{
    return m_back;
}

// Return the camera velocity vector
glm::vec3 CCamera::GetVelocity()
{
    return m_velocity;
}

void CCamera::Release() {
    
}
