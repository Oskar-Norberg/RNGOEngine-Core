    GatherOpaques(world, context, drawQueue);

    GatherCameras(world, context, drawQueue);
void RNGOEngine::Systems::Core::RenderSystem::GatherOpaques(RNGOEngine::Core::World& world,
                                                            EngineSystemContext& context,
                                                            RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
{
    const auto renderView = world.GetRegistry().view<Components::MeshRenderer>();

    for (const auto& [entity, meshRender] : renderView.each())
    {
        const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                   ? world.GetRegistry().get<Components::Transform>(entity)
                                   : Components::Transform();

        const auto& modelData = context.assetManager->GetModel(meshRender.modelID);
        const auto& nonResolvedMaterial = context.assetManager->GetMaterialManager().GetMaterial(
            meshRender.materialID);
        const auto resolvedMaterial = GetResolvedMaterial(nonResolvedMaterial, context);

        for (const auto& mesh : modelData.meshes)
        {
            drawQueue.opaqueObjects.emplace_back(transform, mesh, resolvedMaterial);
        }
    }
}

void RNGOEngine::Systems::Core::RenderSystem::GatherCameras(RNGOEngine::Core::World& world,
                                                            EngineSystemContext& context,
                                                            RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
{
    const auto cameraView = world.GetRegistry().view<Components::Camera>();
    for (const auto& [entity, camera] : cameraView.each())
    {
        const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                   ? world.GetRegistry().get<Components::Transform>(entity)
                                   : Components::Transform();

        // Copy camera properties
        drawQueue.camera = {
            .transform = transform,
            .fov = camera.fov,
            .nearPlane = camera.nearPlane,
            .farPlane = camera.farPlane
        };
    }
}

void RNGOEngine::Systems::Core::RenderSystem::GatherLights(RNGOEngine::Core::World& world,
    EngineSystemContext& context, RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
{
}

RNGOEngine::Core::Renderer::MaterialSpecification RNGOEngine::Systems::Core::RenderSystem::
GetResolvedMaterial(const RNGOEngine::Core::Renderer::MaterialSpecification& materialSpec,
                    EngineSystemContext& context)
{
    RNGOEngine::Core::Renderer::MaterialSpecification resolvedMaterial;
    resolvedMaterial.shader = materialSpec.shader;

    for (const auto& [name, type, data] : materialSpec.uniforms)
    {
        if (type == RNGOEngine::Core::Renderer::UniformType::Texture)
        {
            const auto resolvedTextureID = context.assetManager->GetTextureManager().GetTexture(
                data.texture.texture);
            resolvedMaterial.uniforms.emplace_back(
                name,
                RNGOEngine::Core::Renderer::UniformType::Texture,
                RNGOEngine::Core::Renderer::UniformData{
                    .texture = {resolvedTextureID, data.texture.slot}}
            );
        }
        else
        {
            resolvedMaterial.uniforms.emplace_back(
                name,
                type,
                data
            );
        }
    }

    return resolvedMaterial;
}