cmake_minimum_required(VERSION 2.8.12)
project(SofaGeneralMeshCollision)

set(HEADER_FILES
    DirectSAP.h
    DirectSAP.inl
    IncrSAP.h
    IncrSAP.inl
    # IntrTriangleOBB.h
    # IntrTriangleOBB.inl
    MeshDiscreteIntersection.h
    MeshDiscreteIntersection.inl
    MeshMinProximityIntersection.h
    # RigidContactMapper.h
    # RigidContactMapper.inl
    # SubsetContactMapper.h
    # SubsetContactMapper.inl
    TriangleOctree.h
    TriangleOctreeModel.h
    config.h
    initGeneralMeshCollision.h
)

set(SOURCE_FILES
    DirectSAP.cpp
    IncrSAP.cpp
    # IntrTriangleOBB.cpp
    MeshDiscreteIntersection.cpp
    MeshMinProximityIntersection.cpp
    # RigidContactMapper.cpp
    # SubsetContactMapper.cpp
    TriangleOctree.cpp
    TriangleOctreeModel.cpp
    initGeneralMeshCollision.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC SofaObjectInteraction SofaRigid SofaBaseCollision SofaMeshCollision)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_GENERAL_MESH_COLLISION")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

sofa_install_targets(SofaGeneral ${PROJECT_NAME} ${PROJECT_NAME})
