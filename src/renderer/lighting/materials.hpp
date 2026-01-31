#ifndef MATERIALS_H_
#define MATERIALS_H_

#include <config_manager.hpp>
#include <material.hpp>
#include <mtl_parser.hpp>

/**
 * @class MaterialManager
 * @brief Singleton class for managing materials.
 *
 * This class is responsible for managing materials and providing access to them.
 * It uses the Singleton design pattern to ensure that only one instance of the
 * class exists. The materials are loaded from a file specified in the configuration.
 */
class MaterialManager
{
public:
    /**
     * @brief Gets the singleton instance of MaterialManager.
     *
     * This method returns the single instance of MaterialManager, creating it
     * if it does not already exist.
     *
     * @return The singleton instance of MaterialManager.
     */
    static MaterialManager* getInstance()
    {
        if (instance == nullptr)
            instance = new MaterialManager();
        return instance;
    }

    /**
     * @brief Gets the material associated with a specific MaterialType.
     *
     * @param type The MaterialType for which to retrieve the material.
     * @return The Material object associated with the specified MaterialType.
     */
    Material getMaterial(MaterialType type) { return m_materials[type]; }

private:
    static MaterialManager* instance;                       /**< The singleton instance of MaterialManager. */
    std::unordered_map<MaterialType, Material> m_materials; /**< The map of materials. */

    /**
     * @brief Deleted copy constructor.
     *
     * The copy constructor is deleted to prevent copying of the singleton instance.
     */
    MaterialManager(const MaterialManager&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     *
     * The copy assignment operator is deleted to prevent copying of the singleton instance.
     */
    MaterialManager& operator=(const MaterialManager) = delete;

    /**
     * @brief Private constructor.
     *
     * The constructor is private to prevent instantiation from outside the class.
     * It loads the materials from the file specified in the configuration.
     */
    MaterialManager() { m_materials = IO::parseMTL(ConfigurationManager::getInstance()->getMaterialsPath()); };

    /**
     * @brief Destructor.
     *
     * The destructor is private to prevent deletion of the singleton instance.
     */
    ~MaterialManager() {};
};

#endif
