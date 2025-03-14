#include "shader_factory.h"

void wv::Shader::Factory::reset()
{
	m_vertexInput.clear();
	m_outputs.clear();

	m_fragments.clear();
	m_executionFunctions.clear();
	m_outputValues.clear();
}

