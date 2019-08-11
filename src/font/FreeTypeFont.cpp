#include "FreeTypeFont.h"

CFreeTypeFont::CFreeTypeFont()
{
	m_isLoaded = false;
}
CFreeTypeFont::~CFreeTypeFont()
{
    Release();
}

/* -----------------------------------------------

Name:	createChar

Params:	iIndex - character index in Unicode.

Result:	Creates one single character (its
		texture).

/* --------------------------------------------- */

inline int next_p2(int n){int res = 1; while(res < n)res <<= 1; return res;}

void CFreeTypeFont::CreateChar(int index, const TextureType &textureType)
{

	FT_Load_Glyph(m_ftFace, FT_Get_Char_Index(m_ftFace, index), FT_LOAD_DEFAULT);

	FT_Render_Glyph(m_ftFace->glyph, FT_RENDER_MODE_NORMAL);
	FT_Bitmap* pBitmap = &m_ftFace->glyph->bitmap;

	int iW = pBitmap->width, iH = pBitmap->rows;
	int iTW = next_p2(iW), iTH = next_p2(iH);

	GLubyte* bData = new GLubyte[iTW*iTH];
	// Copy glyph data and add dark pixels elsewhere
	for (int ch = 0; ch < iTH; ch++) 
		for (int cw = 0; cw < iTW; cw++)
			bData[ch*iTW+cw] = (ch >= iH || cw >= iW) ? 0 : pBitmap->buffer[(iH-ch-1)*iW+cw];
 
	// And create a texture from it

    m_charTextures[index].CreateFromData(bData, iTW, iTH, 8, GL_DEPTH_COMPONENT, textureType, false);
	m_charTextures[index].SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_charTextures[index].SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_charTextures[index].SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_charTextures[index].SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Calculate glyph data
	m_advX[index] = m_ftFace->glyph->advance.x>>6;
	m_bearingX[index] = m_ftFace->glyph->metrics.horiBearingX>>6;
	m_charWidth[index] = m_ftFace->glyph->metrics.width>>6;

	m_advY[index] = (m_ftFace->glyph->metrics.height - m_ftFace->glyph->metrics.horiBearingY)>>6;
	m_bearingY[index] = m_ftFace->glyph->metrics.horiBearingY>>6;
	m_charHeight[index] = m_ftFace->glyph->metrics.height>>6;

	m_newLine = std::max(m_newLine, int(m_ftFace->glyph->metrics.height >> 6));

	// Rendering data, texture coordinates are always the same, so now we waste a little memory
	glm::vec2 vQuad[] =
	{
		glm::vec2(0.0f, float(-m_advY[index]+iTH)),
		glm::vec2(0.0f, float(-m_advY[index])),
		glm::vec2(float(iTW), float(-m_advY[index]+iTH)),
		glm::vec2(float(iTW), float(-m_advY[index]))
	};
	glm::vec2 vTexQuad[] = {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)};

	// Add this char to VBO
	for (int i = 0; i < 4; i++) {
		m_vbo.AddData(&vQuad[i], sizeof(glm::vec2));
		m_vbo.AddData(&vTexQuad[i], sizeof(glm::vec2));
	}
	delete[] bData;
}


// Loads an entire font with the given path sFile and pixel size iPXSize
bool CFreeTypeFont::LoadFont(std::string file, int ipixelSize, const TextureType &textureType)
{

	BOOL bError = FT_Init_FreeType(&m_ftLib);
	
	bError = FT_New_Face(m_ftLib, file.c_str(), 0, &m_ftFace);
	if(bError) {
		char message[1024];
		sprintf(message, "ERROR::FREETYPE: Failed to load font\n%s\n", file.c_str());
		return false;
	}
	FT_Set_Pixel_Sizes(m_ftFace, ipixelSize, ipixelSize);
	m_loadedPixelSize = ipixelSize;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();

	for (int i = 0; i < 128; i++)
		CreateChar(i, textureType);
	m_isLoaded = true;

	FT_Done_Face(m_ftFace);
	FT_Done_FreeType(m_ftLib);
	
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, (void*)(sizeof(glm::vec2)));
	return true;
}

// Prints text at the specified location (x, y) with the given pixel size (iPXSize)
void CFreeTypeFont::Print(CShaderProgram* program, std::string text, int x, int y, int pixelSize)
{
	if(!m_isLoaded)
		return;

	glBindVertexArray(m_vao);
    
	int iCurX = x, iCurY = y;
	if (pixelSize == -1)
		pixelSize = m_loadedPixelSize;
	float fScale = float(pixelSize) / float(m_loadedPixelSize);
	for (int i = 0; i < (int) text.size(); i++) {
		if (text[i] == '\n')
		{
			iCurX = x;
			iCurY -= m_newLine*pixelSize / m_loadedPixelSize;
			continue;
		}
		int iIndex = int(text[i]);
		iCurX += m_bearingX[iIndex] * pixelSize / m_loadedPixelSize;
		if(text[i] != ' ')
		{
			m_charTextures[iIndex].BindTexture2DToTextureType();
			glm::mat4 mModelView = glm::translate(glm::mat4(1.0f), glm::vec3(float(iCurX), float(iCurY), 0.0f));
			mModelView = glm::scale(mModelView, glm::vec3(fScale));
			program->SetUniform("matrices.modelViewMatrix", mModelView);
			// Draw character
			glDrawArrays(GL_TRIANGLE_STRIP, iIndex * 4, 4);
		}

		iCurX += (m_advX[iIndex] - m_bearingX[iIndex])*pixelSize / m_loadedPixelSize;
	}
}


// Print formatted text at the location (x, y) with specified pixel size (iPXSize)
void CFreeTypeFont::Render(CShaderProgram* program, int x, int y, int pixelSize, char* text, ...)
{
    char buf[512];
    va_list ap;
    va_start(ap, text);
    vsprintf(buf, text, ap);
    va_end(ap);
    Print(program, buf, x, y, pixelSize);
}

// Deletes all font textures
void CFreeTypeFont::Release()
{
    for (int i = 0; i < 128; i++)
        m_charTextures[i].Release();
    m_vbo.Release();
    glDeleteVertexArrays(1, &m_vao);
}

// Gets the width of text
int CFreeTypeFont::GetTextWidth(std::string sText, int iPixelSize)
{
    int iResult = 0;
    for (int i = 0; i < (int)sText.size(); i++)
        iResult += m_advX[sText[i]];
    return iResult*iPixelSize / m_loadedPixelSize;
}

// Gets the height of text
int CFreeTypeFont::GetTextHeight(std::string sText) {
    return m_loadedPixelSize;
}
