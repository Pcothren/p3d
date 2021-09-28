#include "mvVertexLayout.h"

namespace DearPy3D {

	struct mvVertexElement
	{
		int                 itemCount = 0;
		size_t              size = 0;
		size_t              offset = 0;
		VkFormat            format = VK_FORMAT_UNDEFINED;
		mvVertexElementType type;
	};

	mvVertexLayout mvCreateVertexLayout(std::vector<mvVertexElementType> elements)
	{
		std::vector<mvVertexElement> velements;
		size_t stride = 0;
		size_t size = 0;

		for (auto& element : elements)
		{
			mvVertexElement newelement;
			newelement.type = element;
			switch (element)
			{

			case mvVertexElementType::Position2D:
				newelement.format = VK_FORMAT_R32G32_SFLOAT;
				newelement.itemCount = 2;
				newelement.size = sizeof(float) * 2;
				break;

			case mvVertexElementType::Position3D:
				newelement.format = VK_FORMAT_R32G32B32_SFLOAT;
				newelement.itemCount = 3;
				newelement.size = sizeof(float) * 3;
				break;

			case mvVertexElementType::Tangent:
				newelement.format = VK_FORMAT_R32G32B32_SFLOAT;
				newelement.itemCount = 3;
				newelement.size = sizeof(float) * 3;
				break;

			case mvVertexElementType::Bitangent:
				newelement.format = VK_FORMAT_R32G32B32_SFLOAT;
				newelement.itemCount = 3;
				newelement.size = sizeof(float) * 3;
				break;

			case mvVertexElementType::Normal:
				newelement.format = VK_FORMAT_R32G32B32_SFLOAT;
				newelement.itemCount = 3;
				newelement.size = sizeof(float) * 3;
				break;

			case mvVertexElementType::Texture2D:
				newelement.format = VK_FORMAT_R32G32_SFLOAT;
				newelement.itemCount = 2;
				newelement.size = sizeof(float) * 2;
				break;

			case mvVertexElementType::Color:
				newelement.format = VK_FORMAT_R32G32B32_SFLOAT;
				newelement.itemCount = 3;
				newelement.size = sizeof(float) * 3;
				break;

			}

			newelement.offset = stride;
			stride += newelement.size;
			velements.push_back(newelement);
		}

		mvVertexLayout vertexLayout{};

		VkVertexInputBindingDescription description{};
		description.binding = 0;
		description.stride = stride;
		description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vertexLayout.bindingDescriptions.push_back(description);

		int index = 0;
		for (const auto& element : velements)
		{
			VkVertexInputAttributeDescription description{};
			description.binding = 0;
			description.location = index++;
			description.format = element.format;
			description.offset = element.offset;

			vertexLayout.attributeDescriptions.push_back(description);
		}

		return vertexLayout;
	}
}