#pragma once
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace GameEngine
{
	//TODO - if needed, move out of Engine part to some sort of loader on the game side of things
	namespace eTexture
	{
		enum type
		{
			None = -1,

			Fish, // Replace this for your own enumerations

			Lettuce,

			FishRight,

			LettuceRight,

			Knife,

			Background,

			BrokenBread,

			Bread,

			Title,

			DottedLine,

			ChiliPepper,

			FishWins,

			LettuceWins,

			Count,

		};
	}	

	inline const char* GetPath(eTexture::type texture)
	{
		switch (texture) {
		case eTexture::Fish: return "fish_tiled.png";
		case eTexture::Lettuce: return "lettuce.png";
		case eTexture::Knife: return "knife.png";
		case eTexture::Background: return "bg.png";
		case eTexture::BrokenBread: return "broken.png";
		case eTexture::Bread: return "bread.png";
		case eTexture::FishRight: return "fish_right.png";
		case eTexture::LettuceRight: return "lettuce_right.png";
		case eTexture::Title: return "title.png";
		case eTexture::ChiliPepper: return "pepper.png";
		case eTexture::DottedLine: return "dotted_line.png";
		case eTexture::FishWins: return "carp_wins.png";
		case eTexture::LettuceWins: return "cabbage_wins.png";
		}
		return "UnknownTexType";
	}

	class TextureManager
	{
	public:
		static TextureManager* GetInstance() { if (!sm_instance) sm_instance = new TextureManager(); return sm_instance; }
		~TextureManager();

		void LoadTextures();
		void UnLoadTextures();

		sf::Texture* GetTexture(eTexture::type texture) const { return m_textures[(int)texture]; }

	private:
		TextureManager();
		static TextureManager* sm_instance;

		sf::Texture* m_textures[eTexture::Count];
	};
}

namespace TextureHelper
{
	sf::Vector2f GetTextureTileSize(GameEngine::eTexture::type texture);
}

