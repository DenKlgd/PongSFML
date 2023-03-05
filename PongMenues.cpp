#include "PongMenues.h"

GUI::Menu mainMenu;
GUI::Menu pauseMenu;
GUI::Menu multiplayerMenu;
GUI::Menu connectMenu;
GUI::Menu waitingForConnectionMenu;
GUI::Menu optionsMenu;
sf::Texture loadingImage;
sf::Texture arrow;
uint8_t currentResolutionIndex = 0;
uint8_t prevResolutionIndex = 0;

constexpr uint8_t resolutionCount = sizeof(resolutionList) / sizeof(resolutionList[0]);

void initMainMenu()
{
	mainMenu.renderWindow = &mainRenderWindow;
	mainMenu.loadFont(&font);

	mainMenu.addLabel("PONG", { 0.f, 90.f }, { 200, 50 }, static_cast<GUI::ElementID>(MainMenuElements::PONG_TITLE));
	mainMenu.addButton("1 vs 1", { 0.f, 160.f }, { 200.f, 50.f }, static_cast<GUI::ElementID>(MainMenuElements::PVP));
	mainMenu.addButton("Multiplayer", { 0.f, 220.f }, { 200.f, 50.f }, static_cast<GUI::ElementID>(MainMenuElements::MULTIPLAYER));
	mainMenu.addButton("Settings", { 0.f, 280.f }, { 200.f, 50.f }, static_cast<GUI::ElementID>(MainMenuElements::SETTINGS));
	mainMenu.addButton("Exit", { 0.f, 340.f }, { 200.f, 50.f }, static_cast<GUI::ElementID>(MainMenuElements::EXIT));
	
	mainMenu.alignElementsCenter(windowSize);

	GUI::Label* label = (GUI::Label*)mainMenu.getElementByID(static_cast<GUI::ElementID>(MainMenuElements::PONG_TITLE));
	label->setColor(sf::Color(20, 160, 132), sf::Color(20, 160, 132));

	GUI::Button* button = (GUI::Button*)mainMenu.getElementByID(static_cast<GUI::ElementID>(MainMenuElements::PVP));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)mainMenu.getElementByID(static_cast<GUI::ElementID>(MainMenuElements::MULTIPLAYER));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)mainMenu.getElementByID(static_cast<GUI::ElementID>(MainMenuElements::SETTINGS));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)mainMenu.getElementByID(static_cast<GUI::ElementID>(MainMenuElements::EXIT));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
}

void initPauseMenu()
{
	pauseMenu.renderWindow = &mainRenderWindow;
	pauseMenu.loadFont(&font);

	pauseMenu.addButton("Resume", { 0, (float)windowSize.y / 2 - 50}, { 200, 50 }, static_cast<GUI::ElementID>(PauseMenuElements::RESUME));
	pauseMenu.addButton("Exit to menu", { 0, (float)windowSize.y / 2 + 50 }, { 250, 50 }, static_cast<GUI::ElementID>(PauseMenuElements::RETURN_TO_MENU));

	pauseMenu.alignElementsCenter(windowSize);

	GUI::Button* button = (GUI::Button*)pauseMenu.getElementByID(static_cast<GUI::ElementID>(PauseMenuElements::RESUME));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)pauseMenu.getElementByID(static_cast<GUI::ElementID>(PauseMenuElements::RETURN_TO_MENU));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
}

void initConnectMenu()
{
	connectMenu.loadFont(&font);
	connectMenu.renderWindow = &mainRenderWindow;

	connectMenu.addInputBox({ 0.f, 100.f }, { 250, 50 }, static_cast<GUI::ElementID>(ConnectionMenuElements::IP_TEXTBOX), 15, L"Enter IP");
	connectMenu.addButton("Connect", { 0, 230 }, { 200, 50 }, static_cast<GUI::ElementID>(ConnectionMenuElements::CONNECT));
	connectMenu.addButton("Back", { 0, 310 }, { 200, 50 }, static_cast<GUI::ElementID>(ConnectionMenuElements::RETURN_TO_MENU));

	connectMenu.alignElementsCenter(windowSize);

	GUI::InputBox* inputBox = (GUI::InputBox*)connectMenu.getElementByID(static_cast<GUI::ElementID>(ConnectionMenuElements::IP_TEXTBOX));
	inputBox->keyboard = &keyboardHandler;
	inputBox->filterFunction = [](wchar_t symbol) -> bool {
		if (symbol >= L'0' && symbol <= L'9' || symbol == L'.')
			return true;
		return false;
	};

	GUI::Button* button = (GUI::Button*)connectMenu.getElementByID(static_cast<GUI::ElementID>(ConnectionMenuElements::CONNECT));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)connectMenu.getElementByID(static_cast<GUI::ElementID>(ConnectionMenuElements::RETURN_TO_MENU));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
}

void initMultiplayerMenu()
{
	multiplayerMenu.loadFont(&font);
	multiplayerMenu.renderWindow = &mainRenderWindow;

	multiplayerMenu.addButton("Host", { 0, 150 }, { 200, 50 }, static_cast<GUI::ElementID>(MultiplayerMenuElements::HOST));
	multiplayerMenu.addButton("Connect", { 0, 230 }, { 200, 50 }, static_cast<GUI::ElementID>(MultiplayerMenuElements::CONNECT));
	multiplayerMenu.addButton("Back", { 0, 310 }, { 200, 50 }, static_cast<GUI::ElementID>(MultiplayerMenuElements::RETURN_TO_MENU));

	multiplayerMenu.alignElementsCenter(windowSize);

	GUI::Button* button = (GUI::Button*)multiplayerMenu.getElementByID(static_cast<GUI::ElementID>(MultiplayerMenuElements::HOST));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)multiplayerMenu.getElementByID(static_cast<GUI::ElementID>(MultiplayerMenuElements::CONNECT));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)multiplayerMenu.getElementByID(static_cast<GUI::ElementID>(MultiplayerMenuElements::RETURN_TO_MENU));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
}

void initWaitingForConnectionMenu()
{
	waitingForConnectionMenu.renderWindow = &mainRenderWindow;
	waitingForConnectionMenu.loadFont(&font);

	loadingImage.loadFromFile("Misc/spinner.png");
	waitingForConnectionMenu.addLabel("Waiting for other player", { 0.f, 70.f }, { 200, 50 }, static_cast<GUI::ElementID>(WaitingForConnectionMenu::Title));
	waitingForConnectionMenu.addButton("Cancel", { 0.f, 270.f }, { 200.f, 50.f }, static_cast<GUI::ElementID>(WaitingForConnectionMenu::Cancel));

	waitingForConnectionMenu.alignElementsCenter(windowSize);
	waitingForConnectionMenu.addImageBox({ 400.f, 200.f }, loadingImage, static_cast<GUI::ElementID>(WaitingForConnectionMenu::LoadingCircle));

	GUI::ImageGUIelement* img = (GUI::ImageGUIelement*)waitingForConnectionMenu.getElementByID(static_cast<GUI::ElementID>(WaitingForConnectionMenu::LoadingCircle));
	img->setScale(0.7f, 0.7f);
	img->setRotation(-5);

	GUI::Label* label = (GUI::Label*)waitingForConnectionMenu.getElementByID(static_cast<GUI::ElementID>(WaitingForConnectionMenu::Title));
	label->setColor(sf::Color(20, 160, 132), sf::Color(20, 160, 132));

	GUI::Button* button = (GUI::Button*)waitingForConnectionMenu.getElementByID(static_cast<GUI::ElementID>(WaitingForConnectionMenu::Cancel));
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
}

void initOptionsMenu()
{
	optionsMenu.renderWindow = &mainRenderWindow;
	optionsMenu.loadFont(&font);
	arrow.loadFromFile("Misc\\left-arrow.png");
	
	optionsMenu.addLabel("Resolution", { 300, 130 }, {150, 50}, (GUI::ElementID)SettingsMenu::Title);
	optionsMenu.addLabel(resolutionList[0].first, { 350, 190 }, {200, 50}, (GUI::ElementID)SettingsMenu::ScreenRes);
	optionsMenu.addButton("Apply", { 300, 250 }, {100, 50}, (GUI::ElementID)SettingsMenu::Apply);
	optionsMenu.addButton("Back", { 300, 310 }, {100, 50}, (GUI::ElementID)SettingsMenu::RETURN_TO_MENU);
	optionsMenu.alignElementsCenter(windowSize);

	optionsMenu.addImageButton({ 260, 215 }, arrow, (GUI::ElementID)SettingsMenu::ArrowLeft);
	optionsMenu.addImageButton({ 540, 215 }, arrow, (GUI::ElementID)SettingsMenu::ArrowRight);

	GUI::Label* label = (GUI::Label*)optionsMenu.getElementByID(0);
	label->setColor(sf::Color(20, 160, 132), sf::Color(20, 160, 132));

	GUI::ImageButton* guiArrow = (GUI::ImageButton*)optionsMenu.getElementByID(1);
	guiArrow->setScale(0.5, 0.5);
	
	guiArrow = (GUI::ImageButton*)optionsMenu.getElementByID(2);
	guiArrow->rotate(180);
	guiArrow->setScale(0.5, 0.5);

	GUI::Button* button = (GUI::Button*)optionsMenu.getElementByID(4);
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
	button = (GUI::Button*)optionsMenu.getElementByID(5);
	button->setInteractiveColors(sf::Color::Green, sf::Color::Red);
}

void handleMainMenuInput(GUI::ElementID elementID)
{
	MainMenuElements _elementID = static_cast<MainMenuElements>(elementID);
	switch (_elementID)
	{
	case MainMenuElements::PVP:
		gameState.setImmediately(EGameState::InGame);
		pong = new Pong;
		pong->initGame();
		break;
	case MainMenuElements::MULTIPLAYER:
		gameState.setImmediately(EGameState::MultiplayerMenu);
		break;
	case MainMenuElements::SETTINGS:
		gameState.setImmediately(EGameState::SETTINGS);
		break;
	case MainMenuElements::EXIT:
		if (network != nullptr)
		{
			network->closeConnection();
			delete network;
			network = nullptr;
		}
		mainRenderWindow.close();
		break;
	default:
		break;
	}
}

void handlePauseMenuInput(GUI::ElementID elementID)
{
	PauseMenuElements _elementID = static_cast<PauseMenuElements>(elementID);
	switch (_elementID)
	{
	case PauseMenuElements::RESUME:
		if (!network)
		{
			gameState.setImmediately(EGameState::InGame);
			pong->ResetGameClock();
		}
		else
		{
			gameState.requestGameStateChange(EGameState::InMultiplayer);
		}
		break;
	case PauseMenuElements::RETURN_TO_MENU:
		if (!network)
		{
			gameState.setImmediately(EGameState::Menu);
			delete pong;
			pong = nullptr;
		}
		else
		{
			gameState.requestGameStateChange(EGameState::Menu);
		}
		break;
	default:
		break;
	}
}

void handleConnectMenuInput(GUI::ElementID elementID)
{
	std::string msg;
	GUI::InputBox* ip = nullptr;
	ConnectionMenuElements _elementID = static_cast<ConnectionMenuElements>(elementID);

	switch (_elementID)
	{
	case ConnectionMenuElements::CONNECT:
		ip = (GUI::InputBox*)connectMenu.getElementByID(static_cast<GUI::ElementID>(ConnectionMenuElements::IP_TEXTBOX));
		for (auto iter : ip->getText())
			msg += (char)iter;
		
		udpSock.setBlocking(false);
		udpSock.unbind();
		remoteIP = msg;
		port = 53000;
		remoteIPbuffer = remoteIP;
		connectionStatus = ConnectionStatus::RequestingConnection;
		gameState.setImmediately(EGameState::JoiningToHost);
		break;
	case ConnectionMenuElements::RETURN_TO_MENU:
		gameState.setImmediately(EGameState::MultiplayerMenu);
		break;
	default:
		break;
	}
}

void pauseMenuNetworkUpdate()
{
	if (network == nullptr)
		return;

	std::string data = "";

	gameState.applyGameStateChange();
	EGameState tmpGameState = gameState.getGameState();
	data.append((char*)&tmpGameState, sizeof(EGameState));
	network->sendMessage(data);

	data.clear();
	network->receiveMessage(data);
	data.copy((char*)&tmpGameState, sizeof(EGameState));

	if (tmpGameState != EGameState::Pause)
		gameState.setImmediately(tmpGameState);

	if (gameState.getGameState() == EGameState::Menu)
	{
		delete pong;
		pong = nullptr;
		network->closeConnection();
		delete network;
		network = nullptr;
		return;
	}

	pong->ResetGameClock();
}

void handleMultiplayerMenuInput(GUI::ElementID elementID)
{
	MultiplayerMenuElements _elementID = static_cast<MultiplayerMenuElements>(elementID);

	switch (_elementID)
	{
	case MultiplayerMenuElements::HOST:
		udpSock.setBlocking(false);
		udpSock.unbind();
		remoteIP = sf::IpAddress::Any;
		port = 53000;
		udpSock.bind(port, remoteIP);
		connectionStatus = ConnectionStatus::WaitingForConnections;
		gameState.setImmediately(EGameState::WaitingForOtherPlayer);
		break;
	case MultiplayerMenuElements::CONNECT:
		gameState.setImmediately(EGameState::ConnectMenu);
		break;
	case MultiplayerMenuElements::RETURN_TO_MENU:
		gameState.setImmediately(EGameState::Menu);
		break;
	default:
		break;
	}
}

void handleWaitingForConnectionMenu(GUI::ElementID elementID)
{
	WaitingForConnectionMenu _elementID = static_cast<WaitingForConnectionMenu>(elementID);

	switch (_elementID)
	{
	case WaitingForConnectionMenu::Cancel:
		delete network;
		network = nullptr;
		gameState.setImmediately(EGameState::MultiplayerMenu);
		break;
	default:
		break;
	}
}

void handleOptionsMenu(GUI::ElementID elementID)
{
	SettingsMenu _elementID = (SettingsMenu)elementID;
	GUI::Label* label = (GUI::Label*)optionsMenu.getElementByID((GUI::ElementID)SettingsMenu::ScreenRes);

	switch (_elementID)
	{
	case SettingsMenu::ArrowLeft:
		currentResolutionIndex == 0 ? currentResolutionIndex = resolutionCount - 1 : currentResolutionIndex--;
		label->setText(resolutionList[currentResolutionIndex].first);
		break;
	case SettingsMenu::ArrowRight:
		currentResolutionIndex == resolutionCount - 1 ? currentResolutionIndex = 0 : currentResolutionIndex++;
		label->setText(resolutionList[currentResolutionIndex].first);
		break;
	case SettingsMenu::Apply:
		if (currentResolutionIndex == prevResolutionIndex)
			return;
		mainRenderWindow.setSize({ resolutionList[currentResolutionIndex].second.x, resolutionList[currentResolutionIndex].second.y });
		mainRenderWindow.setView(sf::View(sf::FloatRect(0, 0, resolutionList[currentResolutionIndex].second.x, resolutionList[currentResolutionIndex].second.y)));
		scale = { 
			(float)resolutionList[currentResolutionIndex].second.x / resolutionList[0].second.x,
			(float)resolutionList[currentResolutionIndex].second.y / resolutionList[0].second.y
		};
		windowSize.x = resolutionList[currentResolutionIndex].second.x;
		windowSize.y = resolutionList[currentResolutionIndex].second.y;
		resize();
		break;
	case SettingsMenu::RETURN_TO_MENU:
		gameState.setImmediately(EGameState::Menu);
		break;
	default:
		break;
	}
}

void resize()
{
	uint8_t prevIndex = prevResolutionIndex;
	prevResolutionIndex = currentResolutionIndex;

	float scaleX = 1;
	float scaleY = 1;

	if (currentResolutionIndex > prevIndex)
		for (uint8_t i = prevIndex; i < currentResolutionIndex; i++)
		{
			scaleX *= (float)resolutionList[i + 1].second.x / resolutionList[i].second.x;
			scaleY *= (float)resolutionList[i + 1].second.y / resolutionList[i].second.y;
		}
	else
		for (uint8_t i = prevIndex; i > currentResolutionIndex; i--)
		{
			scaleX *= (float)resolutionList[i - 1].second.x / resolutionList[i].second.x;
			scaleY *= (float)resolutionList[i - 1].second.y / resolutionList[i].second.y;
		}

	mainMenu.setScale(scaleX, scaleY);
	multiplayerMenu.setScale(scaleX, scaleY);
	connectMenu.setScale(scaleX, scaleY);
	waitingForConnectionMenu.setScale(scaleX, scaleY);
	pauseMenu.setScale(scaleX, scaleY);
	optionsMenu.setScale(scaleX, scaleY);
}