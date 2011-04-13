#include <iostream>
#include "viewport.h"
#include "protocol.h"
#include "gameobject.h"
#include "character.h"

viewportManager_t::viewportManager_t(protocol_t &protocol):
  m_protocol(protocol) {}

void viewportManager_t::generate(gameObject_t &object) { _PROFILE;
	for(std::list<gameObject_t*>::iterator i = m_objectList.begin(); i != m_objectList.end(); ++i) {
		gameObject_t *checkedObject = (*i);

		if(&object == checkedObject || checkedObject->mapId() != object.mapId()) {
			continue;
		}

		unsigned int distance = object.calculateDistance(*checkedObject);

		if(distance <= checkedObject->viewRange()) {
			if(checkedObject->viewport().insert(&object).second) {
				//std::cout << "[viewportManager_t::generate()] " << *checkedObject << " saw " << object << ".\n";

				// -----------------------------------------------------------------------------------
				if(checkedObject->type() == gameObject_e::_character) {
					character_t &character = reinterpret_cast<character_t&>(*checkedObject);

					viewport_t viewed;
					viewed.insert(&object);

					if(object.type() == gameObject_e::_character) {
						m_protocol.sendViewportCharacterCreateRequest(character.owner(), viewed);
					} else {
						m_protocol.sendViewportMonsterCreateRequest(character.owner(), viewed);
					}
				}
				// -----------------------------------------------------------------------------------
			}
		}

		if(distance <= object.viewRange()) {
			if(object.viewport().insert(checkedObject).second) {
				//std::cout << "[viewportManager_t::generate()] " << object << " saw " << *checkedObject << ".\n";

				// jest to tymczasowy HACK, poniewaz klient webzen renderuje smieci
				// kiedy po pierwszym wejsciu postaci wysylamy mu liste widzianych obiektow.
				// Trzeba pojedynczo. Byc moze na nowszych klientach nie ma tego problemu.
				// -------------------------------------------------------------------------------
				if(object.type() == gameObject_e::_character) {
					character_t &character = reinterpret_cast<character_t&>(object);

					viewport_t viewed;
					viewed.insert(checkedObject);

					if(checkedObject->type() == gameObject_e::_character) {
						m_protocol.sendViewportCharacterCreateRequest(character.owner(), viewed);
					} else {
						m_protocol.sendViewportMonsterCreateRequest(character.owner(), viewed);
					}
				}
				// -------------------------------------------------------------------------------
			}
		}
	}
}

void viewportManager_t::update(gameObject_t &object) { _PROFILE;
	for(viewport_t::iterator i = object.viewport().begin(); i != object.viewport().end();) {
		gameObject_t *checkedObject = (*i);
		unsigned int distance = object.calculateDistance(*checkedObject);

		if(distance > checkedObject->viewRange() || checkedObject->mapId() != object.mapId()) {
			viewport_t::iterator it = checkedObject->viewport().find(&object);

			if(it != checkedObject->viewport().end()) {
				//std::cout << "[viewportManager_t::update()] " << *checkedObject << " forgot " << object << ".\n";

				// --------------------------------------------------------------------------
				if(checkedObject->type() == gameObject_e::_character) {
					character_t &character = reinterpret_cast<character_t&>(*checkedObject);

					viewport_t forgot;
					forgot.insert(&object);
					m_protocol.sendViewportDestroyRequest(character.owner(), forgot);
				}
				// --------------------------------------------------------------------------

				checkedObject->viewport().erase(it);
			}
		}

		if(distance > object.viewRange() || object.mapId() != checkedObject->mapId()) {
			//std::cout << "[viewportManager_t::update()] " << object << " forgot " << *checkedObject << ".\n";

			i = object.viewport().erase(i);

			viewport_t forgot;
			forgot.insert(checkedObject);

			// ----------------------------------------------------------------------
			if(object.type() == gameObject_e::_character) {
				character_t &character = reinterpret_cast<character_t&>(object);
				m_protocol.sendViewportDestroyRequest(character.owner(), forgot);
			}
			// ----------------------------------------------------------------------
		} else {
			++i;
		}
	}

	this->generate(object);
}

void viewportManager_t::clear(gameObject_t &object) { _PROFILE;
	for(viewport_t::iterator i = object.viewport().begin(); i != object.viewport().end(); ++i) {
		gameObject_t *checkedObject = (*i);
		viewport_t::iterator it = checkedObject->viewport().find(&object);

		if(it != checkedObject->viewport().end()) {
			//std::cout << "[viewportManager_t::clear()] " << *checkedObject << " forgot " << object << ".\n";

			// --------------------------------------------------------------------------
			if(checkedObject->type() == gameObject_e::_character) {
				character_t &character = reinterpret_cast<character_t&>(*checkedObject);

				viewport_t forgot;
				forgot.insert(&object);
				m_protocol.sendViewportDestroyRequest(character.owner(), forgot);
			}
			// --------------------------------------------------------------------------

			checkedObject->viewport().erase(it);
		}

		//std::cout << "[viewportManager_t::clear()] " << object << " forgot " << *checkedObject << ".\n";
	}

	// --------------------------------------------------------------------------------------
	if(object.type() == gameObject_e::_character) {
		character_t &character = reinterpret_cast<character_t&>(object);

		if(character.active()) {
			m_protocol.sendViewportDestroyRequest(character.owner(), character.viewport());
		}
	}
	// --------------------------------------------------------------------------------------

	object.viewport().clear();
}