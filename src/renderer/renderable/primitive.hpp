#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_


#include <vector>
#include <sstream>
#include <renderable.hpp>


/**
 * @class Primitive
 * @brief Base class for renderable primitives.
 *
 * This class provides a base for different types of renderable primitives,
 * such as cubes and spheres. It defines the interface for computing vertices
 * and indices, which must be implemented by derived classes.
 */
class Primitive : public Renderable {
    public:
        /**
         * @brief Default constructor.
         */
        Primitive() : Renderable() {}
    
        /**
         * @brief Virtual destructor.
         */
        virtual ~Primitive() {}
    
    protected:
        /**
         * @brief Initializes the primitive by computing vertices and indices.
         *
         * This method calls computeVertices() and computeIndices() to get the
         * vertices and indices for the primitive, then sets up the rendering.
         */
        void init() {
            m_vertices = computeVertices();
            m_indices = computeIndices();
            setup();
        }
    
        /**
         * @brief Computes the vertices for the primitive.
         *
         * This is a pure virtual function that must be implemented by derived classes.
         *
         * @return A vector of Vertex objects representing the vertices of the primitive.
         */
        virtual std::vector<Vertex> computeVertices() = 0;
    
        /**
         * @brief Computes the indices for the primitive.
         *
         * This is a pure virtual function that must be implemented by derived classes.
         *
         * @return A vector of unsigned int representing the indices of the primitive.
         */
        virtual std::vector<unsigned int> computeIndices() = 0;
};
    
/**
 * @class Cube
 * @brief Represents a cube primitive.
 *
 * This class defines a cube primitive, including methods to set the scale
 * and compute the vertices and indices for rendering.
 */
class Cube : public Primitive {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes a cube with a default scale of 1.0.
     */
    Cube() : Primitive(), m_scale(1.0f) { init(); }

    /**
     * @brief Constructor with scale.
     *
     * @param scale The scale of the cube.
     */
    Cube(float scale) : m_scale(scale) { init(); }

    /**
     * @brief Sets the scale of the cube.
     *
     * @param value The new scale value.
     */
    void setScale(float value) {
        m_scale = value;
        init();
    }

    /**
     * @brief Gets the scale of the cube.
     *
     * @return The current scale value.
     */
    float getScale() const { return m_scale; }

protected:
    /**
     * @brief Computes the vertices for the cube.
     *
     * @return A vector of Vertex objects representing the vertices of the cube.
     */
    std::vector<Vertex> computeVertices() override;

    /**
     * @brief Computes the indices for the cube.
     *
     * @return A vector of unsigned int representing the indices of the cube.
     */
    std::vector<unsigned int> computeIndices() override;

private:
    float m_scale{1.0f}; /**< The scale of the cube. */
};
    
/**
 * @class Sphere
 * @brief Represents a sphere primitive.
 *
 * This class defines a sphere primitive, including methods to set the radius,
 * stack count, and sector count, and compute the vertices and indices for rendering.
 */
class Sphere : public Primitive {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes a sphere with a default radius of 1.0, stack count of 16,
     * and sector count of 32.
     */
    Sphere() : Primitive(), m_radius(1.0f), m_stackCount(16), m_sectorCount(32) {
        init();
    }

    /**
     * @brief Constructor with radius.
     *
     * @param radius The radius of the sphere.
     */
    Sphere(float radius) : Primitive(), m_radius(radius), m_stackCount(16), m_sectorCount(32) {
        init();
    }

    /**
     * @brief Constructor with stack count, sector count, and radius.
     *
     * @param stackCount The stack count of the sphere.
     * @param sectorCount The sector count of the sphere.
     * @param radius The radius of the sphere.
     */
    Sphere(int stackCount, int sectorCount, float radius)
        : Primitive(), m_radius(radius), m_stackCount(stackCount), m_sectorCount(sectorCount) {
        init();
    }

    /**
     * @brief Sets the stack count of the sphere.
     *
     * @param value The new stack count value.
     */
    void setStackCount(int value) { m_stackCount = value; }

    /**
     * @brief Sets the sector count of the sphere.
     *
     * @param value The new sector count value.
     */
    void setSectorCount(int value) { m_sectorCount = value; }

    /**
     * @brief Sets the radius of the sphere.
     *
     * @param value The new radius value.
     */
    void setRadius(float value) { m_radius = value; }

protected:
    /**
     * @brief Computes the vertices for the sphere.
     *
     * @return A vector of Vertex objects representing the vertices of the sphere.
     */
    std::vector<Vertex> computeVertices() override;

    /**
     * @brief Computes the indices for the sphere.
     *
     * @return A vector of unsigned int representing the indices of the sphere.
     */
    std::vector<unsigned int> computeIndices() override;

private:
    int m_stackCount{16};   /**< The stack count of the sphere. */
    int m_sectorCount{32};  /**< The sector count of the sphere. */
    float m_radius{1.0f};   /**< The radius of the sphere. */
};

#endif
