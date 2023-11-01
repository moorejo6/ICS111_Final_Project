// CharacterClasses.h

#pragma once

using namespace System;

namespace CharacterClasses {

	public ref class Character
	{
	private:
		String name;
		int health;
		int magic;

	public:
		Character(){
			name = "test";
			health = 100;
			magic = 100;
		}

		int getHealth(){
			return health;
		}
		
		int getMagic(){
			return magic;
		}

		String getName(){
			return name;
		}
	};
}
