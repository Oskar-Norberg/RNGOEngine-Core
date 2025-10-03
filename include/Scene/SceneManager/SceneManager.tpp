template<Concepts::DerivedFrom<Scene> T>
void SceneManager::LoadScene()
{
    m_pendingScene = std::make_unique<T>();
}