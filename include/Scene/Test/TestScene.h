//
// Created by SkummisDenAndre on 2025-06-17.
//

#pragma once
#include <iostream>

#include "Scene/Scene.h"

class TestScene : public Scene
{
public:
    TestScene() : theEntity(world.CreateEntity())
    {
        theEntity.AddComponent<int>(42);
    }

    void Update(float deltaTime) override
    {
        // theEntity = world.CreateEntity();
        std::cout << "TestScene::Update()" << "\n";
        std::cout << "Entity has int component with value: "
                  << theEntity.GetComponent<int>() << std::endl;
    }

private:
    Entity theEntity;
};
