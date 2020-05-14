#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include "resourceholder.hpp"
#include "resourceidentifiers.hpp"
#include "scenenode.hpp"
#include "spritenode.hpp"
#include "player.hpp"
#include "commandqueue.hpp"
#include "command.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	public:
		explicit							World(sf::RenderWindow& window);
		~World();
		void								update(sf::Time dt);
		void                                updateWorldView(sf::Time dt);  // sub func of update()
		void								draw();
		sf::View                            getWorldView() { return mWorldView; }
		
		CommandQueue&						getCommandQueue() { return mCommandQueue; }

		Player*                             getPlayer() { return mPlayer; }  // clw modify  TODO

	private:
		void								loadTextures();
		void								buildScene();
		void								adaptPlayerPosition();
		void								adaptPlayerVelocity();


	private:
		enum Layer
		{
			Background,
			Land,
			LayerCount
		};


	private:


		sf::RenderWindow&			        mWindow;
		sf::View							mWorldView;
		TextureHolder						mTextures;

		SceneNode							mSceneGraph;
		std::array<SceneNode*, LayerCount>	mSceneLayers;
		CommandQueue						mCommandQueue;

		sf::FloatRect						mWorldBounds;  // TODO：暂时没有用到，而是取场景地图图片纹理的宽高，来判断边界；后期考虑用这个
		sf::Vector2f						mSpawnPosition;
		float								mScrollSpeed;
		Player*							    mPlayer;

};

#endif // BOOK_WORLD_HPP
