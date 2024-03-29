#pragma once

#include <SFML/Audio.hpp>

#include <string>
#include <functional>


namespace sfext {

	using FadeFunc = std::function<float(sf::Time const &, float)>;

	enum class FadeMode {
		None, In, Out
	};

	struct Fader {
		Fader();

		bool operator()(sf::Time const & elapsed, sf::SoundSource& sound, FadeMode mode, float max_volume = 100.f);

		FadeFunc in, out;
	};

	// ---------------------------------------------------------------------------

	class MusicManager {
	private:
		enum FadeState {
			Idle, AtoB, BtoA
		};

		struct Channel {
			sf::Music music;
			sfext::FadeMode mode;

			Channel();
		};

		Channel primary, secondary;
		sfext::Fader fader;
		float max_volume;
		std::string currentFile;

	public:
		MusicManager();
		~MusicManager();

		void play(std::string const & filename);
		void stop();
		void update(sf::Time const & elapsed);

		void setMaxVolume(float volume);
		float getMaxVolume() const;

		sfext::Fader& getFader();
		sf::Music& getCurrent();
	};

}