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
//! \file glc_selectionmaterial.h interface for the GLC_SelectionMaterial class.

#ifndef GLC_SELECTIONMATERIAL_H_
#define GLC_SELECTIONMATERIAL_H_

#include <QColor>
#include <QtOpenGL>
#include <QHash>

#include "../glc_ext.h"
#include "glc_shader.h"

#include "../glc_config.h"

QT_BEGIN_NAMESPACE
class QGLContext;
QT_END_NAMESPACE
class GLC_Material;

//////////////////////////////////////////////////////////////////////
//! \class GLC_SelectionMaterial
/*! \brief GLC_SelectionMaterial : Material used for selection feedback*/

//////////////////////////////////////////////////////////////////////

class GLC_LIB_EXPORT GLC_SelectionMaterial
{
private:
	GLC_SelectionMaterial();


//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Use the given material as selection material
	static void useMaterial(GLC_Material* pMaterial);

	//! Use selection material?
	static void setUseSelectionMaterial(bool useSelectionMaterial);

	//! Use the default selection color
	/*! if a selection material is used, unused it*/
	static void useDefautSelectionColor();

    static void useSelectionColor(const QColor& color);

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Execute OpenGL Material
	static void glExecute();
	//! Init shader
    static void initShader(QOpenGLContext* pContext);
	//! delete shader
    static void deleteShader(QOpenGLContext* pContext);
	//! Set shader
    static void setShaders(QFile& vertex, QFile& fragment, QOpenGLContext* pContext);
	//! Use shader
	static void useShader();
	//! Unused shader
	static void unUseShader();

//@}

//////////////////////////////////////////////////////////////////////
// Private services fonction
//////////////////////////////////////////////////////////////////////
private:
	//! Return the sharing context of the given context
    static QOpenGLContext* sharingContext(QOpenGLContext* pContext);

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////

private:
    //! Selection Shader
    static QHash<QOpenGLContext*, GLC_Shader*> m_SelectionShaderHash;

    //! Selection material id
    static GLC_uint m_SelectionMaterialId;

    //! Material of this selection material
    static GLC_Material* m_pMaterial;

    //! Don't use selection material
    static bool m_NoSelectionMaterial;

    //! Selection Color
    static GLfloat m_DefaultRedComponent;
    static GLfloat m_DefaultGreenComponent;
    static GLfloat m_DefaultBlueComponent;

    static GLfloat m_RedComponent;
    static GLfloat m_GreenComponent;
    static GLfloat m_BlueComponent;

};

#endif /*GLC_SELECTIONMATERIAL_H_*/
