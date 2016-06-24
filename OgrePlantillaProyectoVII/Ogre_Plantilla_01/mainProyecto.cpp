#include "Ogre\ExampleApplication.h"

class FrameListenerClase : public Ogre::FrameListener{
private:
	// Nodo para la cabina
	Ogre::SceneNode* _nodoF01;
	// Nodos para las ruedas
	Ogre::SceneNode* _nodoR01;
	Ogre::SceneNode* _nodoR02;
	Ogre::SceneNode* _nodoR03;
	Ogre::SceneNode* _nodoR04;
	
	OIS::InputManager* _man;
	Ogre::Camera* _cam;
	OIS::Keyboard* _key;
	OIS::Mouse* _mouse;

public:
	FrameListenerClase(Ogre::SceneNode* nodo01, Ogre::SceneNode* nodoR01, Ogre::SceneNode* nodoR02,
						Ogre::SceneNode* nodoR03, Ogre::SceneNode* nodoR04, Ogre::Camera* cam,  RenderWindow* win){
		size_t windowHnd = 0;
		std::stringstream windowHndStr;
		win->getCustomAttribute("WINDOW",&windowHnd);
		windowHndStr << windowHnd;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str())); 

		_man = OIS::InputManager::createInputSystem(pl);
		_key= static_cast<OIS::Keyboard*>(_man->createInputObject(OIS::OISKeyboard,false));
		_mouse= static_cast<OIS::Mouse*>(_man->createInputObject(OIS::OISMouse,false));
		_cam = cam;
		// Nodo para la cabina
		_nodoF01 = nodo01;
		// Nodos para cada rueda
		_nodoR01 = nodoR01;
		_nodoR02 = nodoR02;
		_nodoR03 = nodoR03;
		_nodoR04 = nodoR04;
	
	}

	~FrameListenerClase(){
		_man->destroyInputObject(_key);
		_man->destroyInputObject(_mouse);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent &evt){
		_key->capture();
		_mouse->capture();

		// Inicializamos las variables para el movimiento
		float movSpeed = 10.0f;
		Ogre::Vector3 tmov(0,0,0);
		Ogre::Vector3 tcam(0,0,0);
		float rot = 0.0;
		float rotRueda = 0.0;

		if(_key->isKeyDown(OIS::KC_ESCAPE))
			return false;
		// La nave se mueve hacia el frente
		if(_key->isKeyDown(OIS::KC_W)){
			tcam += Ogre::Vector3(0,0,-10);
			tmov += Ogre::Vector3(0,0,10);
			printf("\n");
			printf("%f", _nodoF01->getPosition().z);
			rotRueda += 10.0;
		}
		// Chequeamos los limites para movimiento lateral de la nave
		if (_nodoF01->getPosition().z < 385.0 && _nodoF01->getPosition().x < 122.4){
			if(_key->isKeyDown(OIS::KC_A)){
				rot = 15.0;
				tcam += Ogre::Vector3(-10,0,0);
				tmov += Ogre::Vector3(10,0,0);
			}
		}
		// Chequeamos los limites para movimiento lateral de la nave
		if (_nodoF01->getPosition().z < 385.0 && _nodoF01->getPosition().x > -122.4){
			if(_key->isKeyDown(OIS::KC_D)){
				rot = -15.0;
				tcam += Ogre::Vector3(10,0,0);
				tmov += Ogre::Vector3(-10,0,0);
			}
		}
		// Movimiento de la camara junto con la nave
		_cam->moveRelative(tcam*movSpeed*evt.timeSinceLastFrame);
		// Si la nave llega al final, reinicia a la posicion inicial
		/*if (_nodoF01->getPosition().z <= -1300){
			tcam = Ogre::Vector3(0,0,0);
			_cam->setPosition(0,8,50);
			_cam->lookAt(0,8,0);
			_nodoF01->setPosition(0,0,0);
			float nuevo = -rot;
			_nodoF01->setOrientation(Quaternion());
			rot = 0.0;
		}*/
		// Movimiento y rotacion de la nave de acuerdo a las teclas presionadas
		_nodoF01->translate(tmov *movSpeed* evt.timeSinceLastFrame);
		_nodoF01->rotate(Ogre::Quaternion(Ogre::Degree(rot*movSpeed* evt.timeSinceLastFrame), Ogre::Vector3(0,1,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		
		_nodoR01->rotate(Ogre::Quaternion(Ogre::Degree(rotRueda*movSpeed* evt.timeSinceLastFrame), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		_nodoR02->rotate(Ogre::Quaternion(Ogre::Degree(rotRueda*movSpeed* evt.timeSinceLastFrame), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		_nodoR03->rotate(Ogre::Quaternion(Ogre::Degree(rotRueda*movSpeed* evt.timeSinceLastFrame), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		_nodoR04->rotate(Ogre::Quaternion(Ogre::Degree(rotRueda*movSpeed* evt.timeSinceLastFrame), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_WORLD);

		return true;
	}

};

class Example1 : public ExampleApplication
{

public:

	Ogre::SceneNode* _nodeChasis01;
	Ogre::SceneNode* _nodeRueda01;
	Ogre::SceneNode* _nodeRueda02;
	Ogre::SceneNode* _nodeRueda03;
	Ogre::SceneNode* _nodeRueda04;
	Ogre::FrameListener* FrameListener01;

	Example1(){
		FrameListener01 = NULL;
	}

	~Example1(){
		if (FrameListener01){
			delete FrameListener01;
		}
	}

	void createFrameListener(){
		FrameListener01 = new FrameListenerClase(_nodeChasis01,_nodeRueda01,_nodeRueda02,_nodeRueda03,_nodeRueda04, mCamera,mWindow);
		mRoot->addFrameListener(FrameListener01);
	}

	void createCamera() {
		/*
		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(0,100,-200);
		mCamera->lookAt(0,0,100);
		mCamera->setNearClipDistance(1);
		*/
		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(0,15,-100);
		mCamera->lookAt(0,15,100);
		mCamera->setNearClipDistance(5);
	}

	void createScene()
	{

		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		
		Ogre::Light* LuzPuntual01 = mSceneMgr->createLight("Luz01");
		LuzPuntual01->setType(Ogre::Light::LT_DIRECTIONAL);
		LuzPuntual01->setDiffuseColour(1.0,1.0,1.0);
		LuzPuntual01->setDirection(Ogre::Vector3( 1, -1, -1 ));
		
		Ogre::Light* LuzPuntual02 = mSceneMgr->createLight("Luz02");
		LuzPuntual02->setType(Ogre::Light::LT_DIRECTIONAL);
		LuzPuntual02->setDiffuseColour(1.0,1.0,1.0);
		LuzPuntual02->setDirection(Ogre::Vector3( -1, -1, -1 ));

		//Chasis
		_nodeChasis01 = mSceneMgr->createSceneNode("Chasis01");
		mSceneMgr->getRootSceneNode()->addChild(_nodeChasis01);
			
		Ogre::Entity* _entChasis01 = mSceneMgr->createEntity("entChasis01", "chasisCarro.mesh");
		_entChasis01->setMaterialName("shCarro01");
		_nodeChasis01->attachObject(_entChasis01);

		//Rueda trasera derecha
		_nodeRueda01 = mSceneMgr->createSceneNode("Rueda01");
		_nodeChasis01->addChild(_nodeRueda01);
			
		Ogre::Entity* _entRueda01 = mSceneMgr->createEntity("entRueda01", "ruedaDetallada.mesh");
		_nodeRueda01->translate(-5.77,3.517,-9.462);
		_entRueda01->setMaterialName("shRueda02");
		_nodeRueda01->attachObject(_entRueda01);

		//Rueda trasera izquierda
		 _nodeRueda02 = mSceneMgr->createSceneNode("Rueda02");
		_nodeChasis01->addChild(_nodeRueda02);
			
		Ogre::Entity* _entRueda02 = mSceneMgr->createEntity("entRueda02", "ruedaDetallada.mesh");
		_nodeRueda02->translate(7.77,3.517,-9.462);
		_entRueda02->setMaterialName("shRueda02");
		_nodeRueda02->attachObject(_entRueda02);

		//Rueda delantera izquierda
		_nodeRueda03 = mSceneMgr->createSceneNode("Rueda03");
		_nodeChasis01->addChild(_nodeRueda03);
			
		Ogre::Entity* _entRueda03 = mSceneMgr->createEntity("entRueda03", "ruedaDetallada.mesh");
		_nodeRueda03->translate(7.77,3.517,9.262);
		_entRueda03->setMaterialName("shRueda02");
		_nodeRueda03->attachObject(_entRueda03);

		//Rueda delantera derecha
		 _nodeRueda04 = mSceneMgr->createSceneNode("Rueda04");
		_nodeChasis01->addChild(_nodeRueda04);
			
		Ogre::Entity* _entRueda04 = mSceneMgr->createEntity("entRueda04", "ruedaDetallada.mesh");
		_nodeRueda04->translate(-5.77,3.517,9.262);
		_entRueda04->setMaterialName("shRueda02");
		_nodeRueda04->attachObject(_entRueda04);


		//BordePista
		Ogre::SceneNode* _nodeBPista = mSceneMgr->createSceneNode("BordePista");
		mSceneMgr->getRootSceneNode()->addChild(_nodeBPista);
				
		Ogre::Entity* _entBPista = mSceneMgr->createEntity("BordePista01", "bordePista.mesh");
		_nodeBPista->attachObject(_entBPista);


		//PisoObstaculo
		Ogre::SceneNode* _nodePObstaculo = mSceneMgr->createSceneNode("PistaObstaculo");
		mSceneMgr->getRootSceneNode()->addChild(_nodePObstaculo);
				
		Ogre::Entity* _entPObstaculo = mSceneMgr->createEntity("PistaObstaculo", "pisoObstaculo01.mesh");
		_nodePObstaculo->attachObject(_entPObstaculo);

		//PisoNOObstaculo
		Ogre::SceneNode* _nodePNObstaculo = mSceneMgr->createSceneNode("PistaNoObstaculo");
		mSceneMgr->getRootSceneNode()->addChild(_nodePNObstaculo);
				
		Ogre::Entity* _entPNOObstaculo = mSceneMgr->createEntity("PistaNoObstaculo", "pisoNoObstaculo01.mesh");
		_nodePNObstaculo->attachObject(_entPNOObstaculo);


		//PosterInicioFinal
		Ogre::SceneNode* _nodePoster = mSceneMgr->createSceneNode("PosterInicioFinal");
		mSceneMgr->getRootSceneNode()->addChild(_nodePoster);
			
		Ogre::Entity* _entPoster = mSceneMgr->createEntity("PosterInicioFinal", "posterInicioFinal.mesh");
		_nodePoster->attachObject(_entPoster);

				
		//BanderaInicial
		Ogre::SceneNode* _nodeBInicial = mSceneMgr->createSceneNode("BanderaInicial");
		mSceneMgr->getRootSceneNode()->addChild(_nodeBInicial);
				
		Ogre::Entity* _entBanderaI = mSceneMgr->createEntity("BanderaInicial", "banderaInicial.mesh");				
		_entBanderaI->setMaterialName("lambert1");				
		_nodeBInicial->attachObject(_entBanderaI);


		//BanderaFinal
		Ogre::SceneNode* _nodeBFinal = mSceneMgr->createSceneNode("BanderaFinal");
		mSceneMgr->getRootSceneNode()->addChild(_nodeBFinal);
		
		Ogre::Entity* _entBanderaF = mSceneMgr->createEntity("BanderaFinal", "banderaFinal.mesh");
		_entBanderaF->setMaterialName("lambert1");
		_nodeBFinal->attachObject(_entBanderaF);
		
		// Creamos la textura de la torreta
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(
			"CilindroText", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::TextureUnitState* torretaTexture =
			mat ->getTechnique(0) ->getPass(0)->createTextureUnitState("coin.jpg");

		//Moneda
		Ogre::SceneNode* _nodeMoneda = mSceneMgr->createSceneNode("Moneda");
		mSceneMgr->getRootSceneNode()->addChild(_nodeMoneda);
		
		Ogre::Entity* _Moneda = mSceneMgr->createEntity("Moneda", "sphere.mesh");
		_Moneda->setMaterial(mat);
		_nodeMoneda->attachObject(_Moneda);
		_nodeMoneda->setScale(0.02,0.02,0.02);
		_nodeMoneda->translate(-5.77,3.517,9.262);
		
		// Fondo estrellado
		mSceneMgr->setSkyBox(true, "matSkyBox", 300);


	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}