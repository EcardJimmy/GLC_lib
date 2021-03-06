/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/
//! \file glc_selectionmaterial.cpp implementation of the GLC_SelectionMaterial class.

#include <QOpenGLContext>
#include <QColor>

#include "glc_selectionmaterial.h"
#include "glc_material.h"


QHash<QOpenGLContext*, GLC_Shader*> GLC_SelectionMaterial::m_SelectionShaderHash;
GLC_uint GLC_SelectionMaterial::m_SelectionMaterialId= 0;
GLC_Material* GLC_SelectionMaterial::m_pMaterial= NULL;
bool GLC_SelectionMaterial::m_NoSelectionMaterial = false;

GLfloat GLC_SelectionMaterial::m_DefaultRedComponent= 1.0f;
GLfloat GLC_SelectionMaterial::m_DefaultGreenComponent= 0.376f;
GLfloat GLC_SelectionMaterial::m_DefaultBlueComponent= 0.223f;

GLfloat GLC_SelectionMaterial::m_RedComponent= GLC_SelectionMaterial::m_DefaultRedComponent;
GLfloat GLC_SelectionMaterial::m_GreenComponent= GLC_SelectionMaterial::m_DefaultGreenComponent;
GLfloat GLC_SelectionMaterial::m_BlueComponent= GLC_SelectionMaterial::m_DefaultBlueComponent;

GLC_SelectionMaterial::GLC_SelectionMaterial()
{

}

void GLC_SelectionMaterial::useMaterial(GLC_Material* pMaterial)
{
	if (0 == m_SelectionMaterialId)
	{
		m_SelectionMaterialId= glc::GLC_GenUserID();
	}
	Q_ASSERT(NULL != pMaterial);
	if (NULL != m_pMaterial)
	{
		m_pMaterial->delUsage(m_SelectionMaterialId);
		if (m_pMaterial->isUnused())
		{
			delete m_pMaterial;
		}
	}
		m_pMaterial= pMaterial;
		m_pMaterial->addUsage(m_SelectionMaterialId);
}

void GLC_SelectionMaterial::useDefautSelectionColor()
{
	if (NULL != m_pMaterial)
	{
		m_pMaterial->delUsage(m_SelectionMaterialId);
		if (m_pMaterial->isUnused())
		{
			delete m_pMaterial;
		}
		m_pMaterial= NULL;
    }
    m_RedComponent= m_DefaultRedComponent;
    m_GreenComponent= m_DefaultGreenComponent;
    m_BlueComponent= m_DefaultBlueComponent;

}

void GLC_SelectionMaterial::useSelectionColor(const QColor &color)
{
    m_RedComponent= static_cast<GLfloat>(color.redF());
    m_GreenComponent= static_cast<GLfloat>(color.greenF());
    m_BlueComponent= static_cast<GLfloat>(color.blueF());
}

// Use selection material?
void GLC_SelectionMaterial::setUseSelectionMaterial(bool useSelectionMaterial) {
	m_NoSelectionMaterial = !useSelectionMaterial;
}


// Execute OpenGL Material
void GLC_SelectionMaterial::glExecute()
{
	if (m_NoSelectionMaterial) return;

	if (NULL != m_pMaterial)
	{
		m_pMaterial->glExecute();
	}
	else
	{
		// Use default selection color
        static GLfloat pAmbientColor[4]= {m_RedComponent, m_GreenComponent, m_BlueComponent, 1.0f};

        static GLfloat pDiffuseColor[4]= {m_RedComponent, m_GreenComponent, m_BlueComponent, 1.0f};

        static GLfloat pSpecularColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};

		static GLfloat pLightEmission[4]= {0.0f, 0.0f, 0.0f, 1.0f};

		static float shininess= 50.0f;

		glColor4fv(pAmbientColor);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pAmbientColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pDiffuseColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecularColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pLightEmission);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
	}
}

void GLC_SelectionMaterial::initShader(QOpenGLContext *pContext)
{
	Q_ASSERT(m_SelectionShaderHash.contains(pContext));
	m_SelectionShaderHash.value(pContext)->createAndCompileProgrammShader();
}

void GLC_SelectionMaterial::setShaders(QFile& vertex, QFile& fragment, QOpenGLContext *pContext)
{
	if (m_SelectionShaderHash.contains(pContext))
	{
		deleteShader(pContext);
	}
	GLC_Shader* pShader= new GLC_Shader;

	pShader->setVertexAndFragmentShader(vertex, fragment);
	m_SelectionShaderHash.insert(pContext, pShader);
}


void GLC_SelectionMaterial::useShader()
{
    if (!m_SelectionShaderHash.isEmpty())
    {
        QOpenGLContext* pContext= QOpenGLContext::currentContext();
        Q_ASSERT(NULL != pContext);
        Q_ASSERT(pContext->isValid());
        if(!m_SelectionShaderHash.contains(pContext))
        {
            pContext= sharingContext(pContext);
            Q_ASSERT(NULL != pContext);
        }

        m_SelectionShaderHash.value(pContext)->use();
    }
}

void GLC_SelectionMaterial::unUseShader()
{
    if (!m_SelectionShaderHash.isEmpty())
    {
        QOpenGLContext* pContext= QOpenGLContext::currentContext();
        Q_ASSERT(NULL != pContext);
        Q_ASSERT(pContext->isValid());
        if(!m_SelectionShaderHash.contains(pContext))
        {
            pContext= sharingContext(pContext);
            Q_ASSERT(NULL != pContext);
        }

        m_SelectionShaderHash.value(pContext)->unuse();
    }
}

//////////////////////////////////////////////////////////////////////
// Private services fonction
//////////////////////////////////////////////////////////////////////
QOpenGLContext *GLC_SelectionMaterial::sharingContext(QOpenGLContext *pContext)
{
    QOpenGLContext* pSharingContext= NULL;
    QHash<QOpenGLContext*, GLC_Shader*>::const_iterator iContext= m_SelectionShaderHash.constBegin();

	while ((NULL == pSharingContext) && (iContext != m_SelectionShaderHash.constEnd()))
	{
        QOpenGLContext* pCurrentContext= iContext.key();
        if (QOpenGLContext::areSharing(pContext, pCurrentContext))
		{
            pSharingContext= pCurrentContext;
		}
		++iContext;
	}

	return pSharingContext;
}

//! delete shader
void GLC_SelectionMaterial::deleteShader(QOpenGLContext *pContext)
{
	Q_ASSERT(m_SelectionShaderHash.contains(pContext));
	GLC_Shader* pShader= m_SelectionShaderHash.value(pContext);
	pShader->deleteShader();
	delete pShader;
	m_SelectionShaderHash.remove(pContext);
}
