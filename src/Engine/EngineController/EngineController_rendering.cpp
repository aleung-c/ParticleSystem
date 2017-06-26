#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Rendering 3D GameObject												//
//																		//
//	This is a classic opengl vertex drawing.							//
//	Faces are loaded from the .obj when you declare a new GameObject	//
//	and everything is set at this point to display the object on the	//
//	screen.																//
//																		//
// --------------------------------------------------------------------	//

void		EngineController::render3DGameObject(GameObject	*obj)
{
	// texture loading.
	if (obj->Texture.HasTexture() == true)
	{
		loadObjectTexture(obj);
	}
	else
		glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_FALSE);
	// opengl buffer loading.
	if (obj->HasModel == true)
	{
		applyMatricesToObject(obj);

		// ------ load the uvs for the object - LOCATION = 1
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetFubo());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// ------ load vertex and draw them - LOCATION = 0
		// ----------- To display triangles from faces vertex
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetFvbo());
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, obj->GetNbFaceVertices());

		// ----------- To display only points from vertex -> useful for debug.
		// glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		// glEnableVertexAttribArray(0);
		// glDrawArrays (GL_POINTS, 0, _objVertices.size());

		// ----- disable all after draw;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void		EngineController::renderMorphAnimation(GameObject	*obj)
{
	// texture loading.
	if (obj->Texture.HasTexture() == true)
	{
		loadObjectTexture(obj);
	}
	else
		glUniform1i(glGetUniformLocation(MorphTargetProgramme, "has_texture"), GL_FALSE);
	// opengl buffer loading.
	if (obj->HasModel == true)
	{
		applyMatricesToObject(obj);
		glUniform1f(glGetUniformLocation(MorphTargetProgramme, "time"),
			obj->MorphAnimation.GetCurTime());

		// ------ load the uvs for the object - LOCATION = 1
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetFubo());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// ------ load vertices for morph target - LOCATION = 2
		glBindBuffer(GL_ARRAY_BUFFER, obj->MorphAnimation.NextFrame->GetFvbo());
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		// ------ load vertex and draw them - LOCATION = 0
		// ----------- To display triangles from faces vertex
		glBindBuffer(GL_ARRAY_BUFFER, obj->MorphAnimation.CurFrame->GetFvbo());
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, obj->GetNbFaceVertices());


		// ----- disable all after draw;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		obj->MorphAnimation.Update();
	}
}

void EngineController::renderBoundingBox(GameObject *obj)
{
	MatModel = glm::mat4();

	MatModel = glm::translate(MatModel, obj->Transform.Position);

	MatModel = glm::scale(MatModel, obj->Transform.Scale);

	MatModel = glm::translate(MatModel, glm::vec3(obj->BoundingBox.LocalCenter));

	// rotation
	// MatModel = glm::rotate(MatModel, glm::radians(obj->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	// MatModel = glm::rotate(MatModel, glm::radians(obj->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	// MatModel = glm::rotate(MatModel, glm::radians(obj->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));

	MatModel = glm::translate(MatModel, glm::vec3(-obj->BoundingBox.LocalCenter));

	// obj->BoundingBox.Update(MatModel);

	// Final MVP matrice merging.
	MatMVP = MatPerspectiveProjection * MatView * MatModel;

	GLint uniform_mat = glGetUniformLocation(MainShaderProgramme, "mvp_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatMVP[0][0]);
	glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_FALSE);

	glBindBuffer(GL_ARRAY_BUFFER, obj->BoundingBox.GetVbo());
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, 8);
	glDisableVertexAttribArray(0);

	
}

// --------------------------------------------------------------------	//
//																		//
//	Rendering UI object													//
//																		//
//	There is only one quad per object, and it is a picture.				//
//	The picture will be drawn using the same shader as the text			//
//	-> orthographic shaders												//
//																		//
// --------------------------------------------------------------------	//

void	EngineController::renderGameUIObject(GameUIObject *obj)
{
	glBindVertexArray(obj->GetVao());
	xpos = obj->Transform.Position.x;
	ypos = obj->Transform.Position.y;

	w = obj->Texture.GetTexture()->width * obj->Transform.ScaleValue;
	h = obj->Texture.GetTexture()->height * obj->Transform.ScaleValue;
	// Creates two triangles forming a quad.
	GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
	};
	// bind character texture, then put the vertices in the object's buffer.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj->Texture.GetTextureID());
	glBindBuffer(GL_ARRAY_BUFFER, obj->GetVbo());
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	// then draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

// --------------------------------------------------------------------	//
//																		//
//	Rendering Text														//
//																		//
//	Text is a bit tricky as every letter is a small quad generated		//
//	from the truetype's loaded font.									//
//																		//
// --------------------------------------------------------------------	//

void	EngineController::renderGameTextObject(GameTextObject *obj)
{
	glUniform3f(glGetUniformLocation(OrthoShaderProgramme, "textColor"),
		obj->Color.x, obj->Color.y, obj->Color.z);
	glBindVertexArray(obj->GetVao());
	tmp_x = obj->Transform.Position.x;
	tmp_y = obj->Transform.Position.y;
	// Iterate through all characters of the string.
	for (std::string::const_iterator c = obj->Text.begin();
		c != obj->Text.end();
		c++)
	{
		ch = Characters[*c]; // take the struct in the map.

		xpos = tmp_x + ch.Bearing.x * obj->Transform.ScaleValue;
		ypos = tmp_y - (ch.Size.y - ch.Bearing.y) * obj->Transform.ScaleValue;

		w = ch.Size.x * obj->Transform.ScaleValue;
		h = ch.Size.y * obj->Transform.ScaleValue;
		// Creates two triangles forming a quad.
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// bind character texture, then put the vertices in the object's buffer.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetVbo());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		// then draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		tmp_x += (ch.Advance >> 6) * obj->Transform.ScaleValue; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glDisableVertexAttribArray(0);
}
