#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	const byte b = pReader->Read<byte>();
	const byte m = pReader->Read<byte>();
	const byte f = pReader->Read<byte>();
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...
	if (b!='B'||m!='M'||f!='F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid.fnt font");
		return nullptr;
	}
	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...
	const byte version = pReader->Read<byte>();
	if (version<3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}
	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize

	byte blockId = pReader->Read<byte>();
	int blockSize = pReader->Read<int>();

	//Retrieve the FontSize [fontDesc.fontSize]

	fontDesc.fontSize = pReader->Read<short>();

	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);
	//Retrieve the FontName [fontDesc.fontName]
	//...

	fontDesc.fontName = pReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize

	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<int>();


	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]

	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();

	//Retrieve PageCount

	short pageCount{ pReader->Read<short>() };

	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)

	if (pageCount > 1)
	{
		Logger::LogError(L"Only one texture per font is allowed!");
	}
	//Advance to Block2 (Move Reader)
	//...

	pReader->MoveBufferPosition(5);
	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize

	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<int>();

	//Retrieve the PageName (BinaryReader::ReadNullString)

	std::wstring pageName{ pReader->ReadNullString() };

	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]

	std::wstring texpath = loadInfo.assetFullPath.parent_path().wstring();
	texpath.append(L"/" + pageName);
	fontDesc.pTexture = ContentManager::Load<TextureData>(texpath);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize

	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<int>();

	//Retrieve Character Count (see documentation)
	
	int charCount{ blockSize / 20 };

	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)

	for (int i=0;i<charCount;++i)
	{
		auto id = static_cast<wchar_t>(pReader->Read<int>());
		
		FontMetric& fontMetric = fontDesc.metrics[id];

		fontMetric.character = id;

		short Xpos = pReader->Read<short>();
		short Ypos = pReader->Read<short>();
		
		fontMetric.width = pReader->Read<short>();
		fontMetric.height = pReader->Read<short>();

		fontMetric.offsetX = pReader->Read<short>();
		fontMetric.offsetY = pReader->Read<short>();

		fontMetric.advanceX = pReader->Read<short>();

		fontMetric.page = pReader->Read<byte>();

		const byte channel = pReader->Read<byte>();
		if(channel&1)
		{
			fontMetric.channel = 2;
		}			
		else if(channel&2)
		{
			fontMetric.channel = 1;
		}
		else if(channel&4)
		{
			fontMetric.channel = 0;
		}
		else if(channel&8)
		{
			fontMetric.channel = 3;
		}
		else if(channel&16)
		{
			fontMetric.channel = 4;
		}
		fontMetric.texCoord.x = static_cast<float>(Xpos) / fontDesc.textureWidth;
		fontMetric.texCoord.y = static_cast<float>(Ypos) / fontDesc.textureHeight;

		fontDesc.metrics[id] = fontMetric;
	}
	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
