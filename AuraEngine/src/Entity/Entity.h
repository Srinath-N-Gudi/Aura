#pragma once
#include "../AuraAPI.h"
#include <string>
#include <vector>
#include <Nyx/Renderer/GL/VAO.h>
#include <memory>
namespace Aura {

	class AEntityManager;
	class ABaseObject;

	class AURA_API Entity {
	private:
		std::string m_Name;
		std::string m_Tag;
	public:
		virtual ~Entity() = default;
		virtual std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO() { return nullptr; } 
		inline std::string getName() const { return m_Name; }
		inline void setName(const std::string& name) { m_Name = name; }
		inline std::string getTag() const { return m_Tag; }
		inline void setTag(const std::string& tag) { m_Tag = tag; }
		inline int getID() const { return m_ID; }
		inline bool IsVisible() const { return m_IsDrawable; }
		inline bool IsAlive() const { return m_IsAlive; }
		inline void setVisible(bool value);
		friend class AEntityManager; // Allow EntityManager to access private members
		friend class ABaseObject;
	protected:
		bool m_IsDrawable = false; // Flag to indicate if the entity is drawable
		bool m_IsAlive = true;     // Flag indicates that the object is alive or not
		int m_ID = -1;

	};
}
