#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLint));
    GLCall(glGenBuffers(1, &m_RendererID));                                                        /* 生成缓冲区 */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));                                           /* 绑定缓冲区 */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); /* 设置缓冲区数据 */
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));                                           /* 绑定缓冲区 */
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));                                           /* 绑定缓冲区 */
}
