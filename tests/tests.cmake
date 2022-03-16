#
# This file lists all integration test sources and test suites
#
target_sources(testprecice
    PRIVATE
    tests/parallel/CouplingOnLine.cpp
    tests/parallel/ExportTimeseries.cpp
    tests/parallel/GlobalRBFPartitioning.cpp
    tests/parallel/LocalRBFPartitioning.cpp
    tests/parallel/MasterSockets.cpp
    tests/parallel/NearestProjectionRePartitioning.cpp
    tests/parallel/TestBoundingBoxInitialization.cpp
    tests/parallel/TestBoundingBoxInitializationTwoWay.cpp
    tests/parallel/TestFinalize.cpp
    tests/parallel/UserDefinedMPICommunicator.cpp
    tests/parallel/UserDefinedMPICommunicatorPetRBF.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshAndMapping.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshEmptyPartition.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshEmptyPartitionTwoLevelInit.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshNoOverlap.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshNoOverlapTwoLevelInit.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshOverlap.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshOverlapNoWrite.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshOverlapNoWriteTwoLevelInit.cpp
    tests/parallel/direct-mesh-access/AccessReceivedMeshOverlapTwoLevelInit.cpp
    tests/parallel/direct-mesh-access/helpers.cpp
    tests/parallel/direct-mesh-access/helpers.hpp
    tests/parallel/distributed-communication/TestDistributedCommunicationGatherScatterMPI.cpp
    tests/parallel/distributed-communication/TestDistributedCommunicationP2PMPI.cpp
    tests/parallel/distributed-communication/TestDistributedCommunicationP2PSockets.cpp
    tests/parallel/distributed-communication/helpers.cpp
    tests/parallel/distributed-communication/helpers.hpp
    tests/parallel/gather-scatter/EnforceGatherScatterEmptyMaster.cpp
    tests/parallel/gather-scatter/EnforceGatherScatterEmptyReceivedMaster.cpp
    tests/parallel/gather-scatter/helpers.cpp
    tests/parallel/gather-scatter/helpers.hpp
    tests/parallel/lifecycle/ConstructAndExplicitFinalize.cpp
    tests/parallel/lifecycle/ConstructOnly.cpp
    tests/parallel/lifecycle/Full.cpp
    tests/parallel/lifecycle/ImplicitFinalize.cpp
    tests/parallel/quasi-newton/TestQN1.cpp
    tests/parallel/quasi-newton/TestQN1EmptyPartition.cpp
    tests/parallel/quasi-newton/TestQN2.cpp
    tests/parallel/quasi-newton/TestQN2EmptyPartition.cpp
    tests/parallel/quasi-newton/TestQN3.cpp
    tests/parallel/quasi-newton/TestQN3EmptyPartition.cpp
    tests/parallel/quasi-newton/helpers.cpp
    tests/parallel/quasi-newton/helpers.hpp
    tests/serial/TestExplicitWithDataMultipleReadWrite.cpp
    tests/serial/TestExplicitWithSolverGeometry.cpp
    tests/serial/explicit/TestExplicitMPI.cpp
    tests/serial/explicit/TestExplicitMPISingle.cpp
    tests/serial/explicit/TestExplicitSockets.cpp
    tests/serial/explicit/helpers.cpp
    tests/serial/explicit/helpers.hpp
    tests/serial/initialize-data/Explicit.cpp
    tests/serial/initialize-data/Implicit.cpp
    tests/serial/initialize-data/ReadMapping.cpp
    tests/serial/initialize-data/WriteMapping.cpp
    tests/serial/initialize-data/helpers.cpp
    tests/serial/initialize-data/helpers.hpp
    tests/serial/lifecycle/ConstructAndExplicitFinalize.cpp
    tests/serial/lifecycle/ConstructOnly.cpp
    tests/serial/lifecycle/Full.cpp
    tests/serial/lifecycle/ImplicitFinalize.cpp
    tests/serial/mesh-requirements/NearestNeighborA.cpp
    tests/serial/mesh-requirements/NearestNeighborB.cpp
    tests/serial/mesh-requirements/NearestProjection2DA.cpp
    tests/serial/mesh-requirements/NearestProjection2DB.cpp
    tests/serial/multi-coupling/MultiCoupling.cpp
    tests/serial/multi-coupling/MultiCouplingFourSolvers1.cpp
    tests/serial/multi-coupling/MultiCouplingFourSolvers2.cpp
    tests/serial/multi-coupling/MultiCouplingThreeSolvers1.cpp
    tests/serial/multi-coupling/MultiCouplingThreeSolvers2.cpp
    tests/serial/multi-coupling/MultiCouplingThreeSolvers3.cpp
    tests/serial/multi-coupling/helpers.cpp
    tests/serial/multi-coupling/helpers.hpp
    tests/serial/time/explicit/DoNothingWithSubcycling.cpp
    tests/serial/time/explicit/ReadWriteScalarDataWithSubcycling.cpp
    tests/serial/time/implicit/ReadWriteScalarDataWithSubcycling.cpp
    tests/serial/whitebox/TestConfigurationComsol.cpp
    tests/serial/whitebox/TestConfigurationPeano.cpp
    tests/serial/whitebox/TestExplicitWithDataScaling.cpp
    )

# Contains the list of integration test suites
set(PRECICE_TEST_SUITES Parallel Serial)
