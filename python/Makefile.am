# python/ Makefile.am


# Spinal codes python implementation (the "reference" implementation)
nobase_dist_pkgpython_PYTHON = \
	./codes/spinal/reference/Example1.py \
	./codes/spinal/reference/Encoder.py \
	./codes/spinal/reference/Hash.py \
	./codes/spinal/reference/Decoder.py \
	./codes/spinal/reference/SymbolMapper.py \
	./codes/spinal/reference/__init__.py \
	./codes/spinal/reference/Lookup3Hash.py \
	./codes/spinal/reference/SalsaHash.py \
	./codes/spinal/reference/RNG.py \
	./simulator/factories/DetectorFactory.py \
	./simulator/factories/codes/TurboFactory.py \
	./simulator/factories/codes/LTFactory.py \
	./simulator/factories/codes/LdpcFactory.py \
	./simulator/factories/codes/SpinalFactory.py \
	./simulator/factories/codes/NullFactory.py \
	./simulator/factories/codes/MultiplexedFactory.py \
	./simulator/factories/codes/__init__.py \
	./simulator/factories/codes/StriderFactory.py \
	./simulator/factories/codes/RaptorFactory.py \
	./simulator/factories/codes/FadingStriderFactory.py \
	./simulator/factories/ProtocolFactory.py \
	./simulator/factories/StatisticsFactory.py \
	./simulator/factories/__init__.py \
	./simulator/factories/PacketGenFactory.py \
	./simulator/factories/DemapFactory.py \
	./simulator/factories/ChannelFactory.py \
	./simulator/factories/MapFactory.py \
	./simulator/default_configuration.py \
	./simulator/FactoryCollection.py \
	./simulator/Simulator.py \
	./simulator/__init__.py \
	./simulator/SanityTest.py \
	./protocols/FastFourwayProtocol.py \
	./protocols/__init__.py \
	./protocols/MultipleTryProtocol.py \
	./statistics/__init__.py \
	./statistics/ErrorRateStatistics.py \
	./statistics/ErrorLocationStatistics.py \
	./statistics/FirstErrorStatistics.py \
	./util/serialization/__init__.py


### Serialization code in util/serialization
serialization_pythondir = ${pkgpythondir}/util/serialization
# use the SCRIPTS target, because the PYTHON target doesn't compile the targets.
serialization_python_SCRIPTS = results_pb2.py
# make rule for protoc
results_pb2.py: util/serialization/results.proto
	protoc --python_out=${builddir} -I${srcdir}/util/serialization $<
	