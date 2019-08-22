#include "Camera.h"

// https://stackoverflow.com/questions/29451567/how-to-move-in-the-direction-of-the-camera-opengl-c
// https://gamedev.stackexchange.com/questions/136236/opengl-camera-movement-with-mouse-buttons
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
// https://www.3dgep.com/understanding-the-view-matrix/

// Constructor for camera -- initialise with some default values
CCamera::CCamera()
{
	m_position = glm::vec3(0.0f, 0.0f, -15.0f);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_view = glm::vec3(0.0f, 0.0f, 0.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_worldUp = glm::vec3( 0.0f, 1.0f, 0.0f );
    m_pitch = 0.0f;
    m_yaw = -90.0f;
    m_roll = 0.0f;
    m_fieldOfView = (GLfloat)FOV;
    m_movementSpeed = (GLfloat)SPEED;
    m_mouseSensitivity = (GLfloat)SENSITIVTY;
    m_screenWidth = (GLfloat)SCREEN_WIDTH;
    m_screenHeight = (GLfloat)SCREEN_HEIGHT;
    m_zNear = (GLfloat)ZNEAR;
    m_zFar = (GLfloat)ZFAR;
    m_viewMatrix = glm::mat4(1.0f);
    m_modelMatrix = glm::mat4(1.0f);
    SetPerspectiveProjectionMatrix(m_fieldOfView, (m_screenWidth/m_screenHeight), m_zNear, m_zFar);
    SetOrthographicProjectionMatrix(m_screenWidth, m_screenHeight, m_zNear, m_zFar);
}
CCamera::~CCamera()
{
    Release();
}
 
void CCamera::Create(
            const glm::vec3 &position,
            const glm::vec3 &front,
            const glm::vec3 &up,
            const glm::vec3 &worldUp,
            const GLfloat &fieldOfView,
            const GLfloat &width,
            const GLfloat &height,
            const GLfloat &zNear,
            const GLfloat &zFar) {
    
    this->m_position = position;
    this->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->m_front = front;
    this->m_up = up;
    this->m_worldUp = worldUp;
    this->m_fieldOfView = fieldOfView;
    this->m_zNear = zNear;
    this->m_zFar = zFar;
    this->SetPerspectiveProjectionMatrix(fieldOfView, (width/height), zNear, zFar);
    this->SetOrthographicProjectionMatrix(width, height, zNear, zFar);
    this->m_pitch = 0.0f;
    this->m_yaw = -90.0f;
    this->m_roll = 0.0f;
    this->m_movementSpeed = (GLfloat)SPEED;
    this->m_mouseSensitivity = (GLfloat)SENSITIVTY;
    this->m_screenWidth = width;
    this->m_screenHeight = height;
    this->UpdateCameraVectors();
}


// Calculates the front vector from the Camera's (updated) Eular Angles
void CCamera::UpdateCameraVectors( )
{
   /*
    http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
    glm::quat yawAngle = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0, 1, 0)); // also known as Heading: rotation about y axis (aka “yaw”)
    glm::quat pitchAngle = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1, 0, 0)); // rotation about x axis
    glm::quat rollAngle = glm::angleAxis(glm::radians(m_roll), glm::vec3(0, 0, 1)); // also known as Bank: rotation about z axis (aka “roll”)
    
     //According to euler angle convention, we first do the heading and rotation about the object space y-axis (pitch)
    //Then we rotate about the object space x-axis (yaw), then we do the object space z-axis (roll)
    glm::quat q = yawAngle * pitchAngle * rollAngle;
    glm::mat4 rot = glm::mat4_cast(q); // quat to rotation matrix
    
    this->m_front = glm::vec3(rot[0][0], rot[0][1], rot[0][2]);//column 3 of your transform. Possibly m11,m12,m13 of the transform in openGL
    this->m_back = glm::normalize(m_front) * -1.0f;
    
    this->m_up = glm::vec3(rot[1][0], rot[1][1], rot[1][2]);//column 3 of your transform. Possibly m21,m22,m23 of the transform in openGL
    this->m_down = glm::normalize(m_up) * -1.0f;
    
    this->m_right = glm::vec3(rot[2][0], rot[2][1], rot[2][2]);//column 3 of your transform. Possibly m31,m32,m33 of the transform in openGL
    this->m_left = glm::normalize(m_right) * -1.0f;
    */
    
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
         m_view, // what target you want the camera to be looking at in World Space, meaning look at what(using vec3) ?  // meaning the camera view point
         m_up  //which direction is up, probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
         );
    
    
    /*
    You need a 3 by 3 rotation matrix to rotate your object: R but if you also add translation terms, transformation matrix will be 4 by 4:
        
        [R11, R12, R13 tx]
        [R21, R22, R23 ty]
        [R31, R32, R33 tz]
        [0,   0,   0,   1]
     
     With those considerations in mind, we set out to solve for the Euler
     angles from the rotation matrix (Equation (8.14)) directly. For your convenience, the matrix is expanded below:
  
     [(cos h cos b + sin h sin p sin b), (sin b cos p), (− sin h cos b + cos h sin p sin b)]
     [(− cos h sin b + sin h sin p cos b), (cos b cos p), (sin b sin h + cos h sin p cos b)]
     [(sin h cos p),  (− sin p), (cos h cos p)].
     We can solve for p immediately from m32:

    */
    /*
    // rotation matrix
    // https://www.youtube.com/watch?v=zc8b2Jo7mno
    glm::mat4 rm = glm::mat4(m_right.x,    m_right.y,      -m_right.z,      0.0f,
                            -m_up.x,       m_up.y,         m_up.z,         0.0f,
                            m_front.x,    -m_front.y,      m_front.z,      0.0f,
                            m_position.x, m_position.y,   m_position.z,     1.0f
    );
  
    // https://stackoverflow.com/questions/16663647/3x3-matrix-rotation-in-c
    // https://learnopengl.com/Getting-started/Transformations
    // https://www.cprogramming.com/tutorial/3d/rotationMatrices.html
    // https://www.learnopencv.com/rotation-matrix-to-euler-angles/
    // https://gamedev.stackexchange.com/questions/50963/how-to-extract-euler-angles-from-transformation-matrix
    GLfloat sinP = -(rm[2][1]);
    GLfloat pitch = 0.0f;           // between -90 < p < 90
    if (sinP <= -1.0f) {
        pitch = -1.570796f;         // - pi / 2
    } else if (sinP >= 1.0f) {
        pitch = 1.570796f;          //  pi / 2
    } else {
        pitch = glm::asin(sinP);    // p is pitch for (-sin p) in m32
    }
    
    // heading is atan2(sinH, cosH), or atan2(m13 / cos(p), m33 / cos(p)), or simply atan2(m13, m33)
    GLfloat heading;            // between -180 < h < 180
    
    // bank is atan2(m21, m22),  if cos p = 0, then p = + or - 90
    GLfloat bank;               // between -180 < b < 180
    
    // check fot the gimbal lock case. giving a slight tolerance for numerical imprecision
    if (fabs(sinP) > 0.99999f) {
        // we are looking straight up or down.
        // slam bank to zero and just set heading
        heading = atan2(-rm[1][2], rm[0][0]);
        bank = 0.0f;
    } else {
        // compute heading from m13 and m33
        heading = atan2(rm[2][0], rm[2][2]);
        
        // compute bank from m21 and m22
        bank = atan2(rm[0][1], rm[1][1]);
    }
    
    const GLfloat pi2 = 3.14159265358979323846264f * 2; // equivelent to 360 degrees
    if(heading < 0.0f) heading += pi2;   // make yaw between 0 and 360 degrees
    if(bank < 0.0f) bank += pi2;         // make roll between 0 and 360 degrees
    
    this->m_pitch = glm::degrees(pitch); //Pitch is between [-90 ... 90] degrees
    this->m_yaw = glm::degrees(heading);
    this->m_roll = glm::degrees(bank);
  
    // I assume the values are already converted to radians.
    GLfloat cosPitch = glm::cos(pitch);
    GLfloat sinPitch = glm::sin(pitch);
    GLfloat cosYaw = glm::cos(heading);
    GLfloat sinYaw = glm::sin(heading);
    
    glm::vec3 xaxis = { cosYaw, 0, -sinYaw };
    glm::vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    glm::vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
    
    // http://www.songho.ca/opengl/gl_camera.html
    // https://www.3dgep.com/understanding-the-view-matrix/
    
     WARNING: NOT WORKING PROPERLY, RESEARCH VIEWMATRIX
    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    this->m_viewMatrix = {
        glm::vec4(       xaxis.x,            yaxis.x,            zaxis.x,      0.0f ),
        glm::vec4(       xaxis.y,            yaxis.y,            zaxis.y,      0.0f ),
        glm::vec4(       xaxis.z,            yaxis.z,            zaxis.z,      0.0f ),
        glm::vec4( -m_right.x * m_position.x - m_right.y * m_position.y - m_right.z * m_position.z,
                  -m_up.x * m_position.x - m_up.y * m_position.y - m_up.z * m_position.z,
                  -m_back.x * m_position.x - m_back.y * m_position.y - m_back.z * m_position.z,
                  1.0f )
    };
    */
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
void CCamera::SetViewByMouse(const MouseState &state)
{
    if (state.m_isEnabled && state.m_isNavigating) {
        
        // https://learnopengl.com/Getting-started/Camera
        m_yaw   += state.xoffset * m_mouseSensitivity;
        m_pitch += state.yoffset * m_mouseSensitivity;
        
        if(m_pitch > 89.0f)
            m_pitch = 89.0f;
        if(m_pitch < -89.0f)
            m_pitch = -89.0f;
        
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        UpdateCameraVectors();
    }
}


// Update the camera to respond to key presses for translation
void CCamera::TranslateByKeyboard(const double &dt, const KeyboardState &keyboardState)
{
    
    double speed = keyboardState.m_isSlowMotion ? m_movementSpeed * 0.02f : m_movementSpeed;
    if (keyboardState.m_arrowKeyDown != ControlType::UNKNOWN){        
        //if (keyPressed == GLFW_KEY_UP)// || keyPressed == GLFW_KEY_W )  // FORWARD
        if (keyboardState.m_arrowKeyDown == ControlType::KEYFORWARD)
        {
            Advance(speed * dt);
        }
        
        //if (keyPressed == GLFW_KEY_DOWN)// || keyPressed == GLFW_KEY_S )  // BACKWARD
        if (keyboardState.m_arrowKeyDown == ControlType::KEYBACKWARD)
        {
            Advance(-speed * dt);
        }
        
        //if (keyPressed == GLFW_KEY_LEFT)// || keyPressed == GLFW_KEY_A )  // LEFT
        if (keyboardState.m_arrowKeyDown == ControlType::KEYLEFT)
        {
            Strafe(-speed * dt);
        }
        
        //if (keyPressed == GLFW_KEY_RIGHT)// || keyPressed == GLFW_KEY_D )   //RIGHT
        if (keyboardState.m_arrowKeyDown == ControlType::KEYRIGHT)
        {
            Strafe(speed * dt);
        }
    }
    
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void CCamera::RotateViewPoint(float fAngle, glm::vec3 &vPoint)
{
	glm::vec3 vView = m_view - m_position;// direction vector
	
	glm::mat4 R = glm::rotate(glm::mat4(1), glm::degrees(fAngle), vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);

    m_front = glm::normalize(glm::vec3(newView));
    
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
    
    m_isMoving = true;
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
    m_isMoving = true;
    UpdateCameraVectors();
}

// Advance the camera (forward / backward motion)
void CCamera::Advance(double direction)
{
	float speed = (float) (SPEEDRATIO * direction);

	glm::vec3 view = glm::normalize(m_view - m_position);
	m_position = m_position + view * speed;
	//m_view = m_view + view * speed;
    m_isMoving = true;
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
void CCamera::Update(GLFWwindow *window, const GLdouble &dt,
                     const MouseState &mouseState, const KeyboardState &keyboardState,
                     const GLboolean &moveCamera)
{
    m_isMoving = false;
	glm::vec3 vector = glm::cross(m_view - m_position, m_up);
	m_strafeVector = glm::normalize(vector);

    if (moveCamera) {
        SetFieldOfView(mouseState.m_scroll);
        SetViewByMouse(mouseState);
        TranslateByKeyboard(dt, keyboardState);
    }
    
    // Update the velocity of the camera
    SetVelocity(dt);
}

// update ot the end of current frame
void CCamera::Reset(GLFWwindow *window, const GLdouble &dt) {
    m_previousPosition = m_position;
    
    // set the previous model->view->matrix of the camera
    glm::mat4 model = m_modelMatrix;
    glm::mat4 view = GetViewMatrix();
    glm::mat4 proj = *GetPerspectiveProjectionMatrix();
    m_prevMVP = proj * view * model;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void CCamera::SetPerspectiveProjectionMatrix(const GLfloat &fieldOfView, const GLfloat &aspectRatio, const GLfloat &nearClippingPlane, const GLfloat &farClippingPlane){
    this->m_perspectiveProjectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void CCamera::SetOrthographicProjectionMatrix(const GLfloat &width, const GLfloat height, const GLfloat &zNear, const GLfloat &zFar){
    this->m_orthographicProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, zNear, zFar);
}

void CCamera::SetOrthographicProjectionMatrix(float value , float zNear, float zFar)
{
    this->m_orthographicProjectionMatrix = glm::ortho(-value, value, -value, value, zNear, zFar);
}

void CCamera::SetOrthographicProjectionMatrix(int width, int height)
{
    m_orthographicProjectionMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void CCamera::SetFieldOfView(const GLfloat &yoffset)
{
    if (m_fieldOfView >= 1.0f && m_fieldOfView <= 65.0f) {
        m_fieldOfView -= yoffset;
    } else if (m_fieldOfView < 1.0f) {
        m_fieldOfView = 1.0f;
    } else if (m_fieldOfView > 65.0f) {
        m_fieldOfView = 65.0f;
    }
    SetPerspectiveProjectionMatrix(m_fieldOfView, (m_screenWidth/m_screenHeight), m_zNear, m_zFar);
}

// Set the model matrix with a new matrix
void CCamera::SetModelMatrix(const glm::mat4 &model) {
    this->m_modelMatrix = model;
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

glm::mat4 CCamera::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, m_yaw, glm::vec3(0.0f, 1.0f, 0.0f)); // also known as Heading: rotation about y axis (aka “yaw”)
    model = glm::rotate(model, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f)); // rotation about x axis
    model = glm::rotate(model, m_roll, glm::vec3(0.0f, 0.0f, 1.0f)); // also known as Bank: rotation about z axis (aka “roll”)
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    return model;
}

// return  the camera previous frame model view projection matrix
glm::mat4 CCamera::GetPreviousMVP() const {
    return m_prevMVP;
}

// Get the camera view matrix
glm::mat4 CCamera::GetViewMatrix() const
{
    return this->m_viewMatrix;
}

glm::mat4 CCamera::GetViewWithoutTranslation() const {
    /*
     The current view matrix however transforms all the skybox's positions by rotating, scaling and translating them,
     so if the player moves, the cubemap moves as well! We want to remove the translation part
     of the view matrix so movement doesn't affect the skybox's position vectors.
     
     we could remove the translation section of transformation matrices by taking the upper-left 3x3 matrix of the 4x4 matrix, effectively removing the translation components. We can achieve this by simply converting the view matrix to a 3x3 matrix (removing translation) and converting it back to a 4x4 matrix:
     
     glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
     
     This removes any translation, but keeps all rotation transformations so the user can still look around the scene.
     */
    return glm::mat4(glm::mat3(this->m_viewMatrix));
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

// Return the far clipping plane
GLfloat CCamera::GetFarPlane() {
    return m_zFar;
}

// Return the near clipping plane
GLfloat CCamera::GetNearPlane() {
    return m_zNear;
}

GLfloat CCamera::GetFieldOfView() {
    return m_fieldOfView;
}

GLfloat CCamera::GetAspectRatio() {
    return (m_screenWidth/m_screenHeight);
}

GLboolean CCamera::IsMoving() const {
    return m_isMoving;
}

void CCamera::Release() {
    
}

