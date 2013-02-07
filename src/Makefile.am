# Components for simulation
noinst_LTLIBRARIES = lib_rf_misc.la   lib_rf_channels.la  lib_rf_mappers.la
noinst_LTLIBRARIES += lib_rf_demappers.la  lib_rf_protocols.la

# Code-related libraries
noinst_LTLIBRARIES += lib_rf_codes.la  lib_rf_fountain.la lib_rf_strider.la  lib_rf_turbo.la  
noinst_LTLIBRARIES += lib_rf_ldpc.la

# Utilities
noinst_LTLIBRARIES += lib_rf_hashes.la lib_rf_inference.la


# Final libraries (spinal codes in its own library)
lib_LTLIBRARIES = libwireless.la libspinal.la

AM_CPPFLAGS += -I$(top_srcdir)/include

# Libwireless
libwireless_la_SOURCES =
libwireless_la_LDFLAGS = -Wl,-Bsymbolic-functions
libwireless_la_LIBADD = lib_rf_misc.la   lib_rf_channels.la  lib_rf_mappers.la \
	lib_rf_demappers.la  lib_rf_protocols.la \
	lib_rf_codes.la  lib_rf_fountain.la lib_rf_strider.la  lib_rf_turbo.la lib_rf_ldpc.la \
	lib_rf_inference.la lib_rf_hashes.la $(ITPP_LIBS)

# Libspinal
libspinal_la_SOURCES = \
	./codes/spinal/CodeFactory.cpp \
	./codes/spinal/StubHashDecoder.cpp \
	./codes/spinal/protocols/StridedProtocol.cpp \
	./codes/spinal/SpinalBranchEvaluator.cpp
libspinal_la_CPPFLAGS = -I$(srcdir)/include -I$(top_srcdir)/include
libspinal_la_LDFLAGS = -Wl,--exclude-libs=ALL -Wl,-Bsymbolic-functions
libspinal_la_LIBADD = -l_rf_hashes -l_rf_mappers -l_rf_codes -l_rf_channels -l_rf_misc $(ITPP_LIBS)

# Individual components
lib_rf_misc_la_SOURCES = \
	./util/ItppUtils.cpp \
	./util/Utils.cpp \
	./util/crc.cpp \
	./util/BitStatCounter.cpp \
	./util/BlockStatCounter.cpp \
	./CrcPacketGenerator.cpp \
	./PacketGenerator.cpp
lib_rf_channels_la_SOURCES = \
	./channels/BscChannel.cpp \
	./channels/CoherenceCoeffGenerator.cpp \
	./channels/MimoChannel.cpp
lib_rf_mappers_la_SOURCES = \
	./mappers/NormalDistribution.cpp \
	./mappers/EvenModulator.cpp \
	./mappers/QPSKMapper.cpp \
	./mappers/TruncatedNormalDistribution.cpp \
	./mappers/GaussianMapper.cpp \
	./mappers/ComplexLinearMapper.cpp \
	./mappers/LinearMapper.cpp \
	./mappers/GrayMapper.cpp \
	./mappers/QamMapper.cpp \
	./mappers/SoftMapper.cpp
lib_rf_demappers_la_SOURCES = \
	./demappers/BscDemapper.cpp \
	./demappers/NullDemapper.cpp
lib_rf_protocols_la_SOURCES = \
	./protocols/RateApproxProtocol.cpp \
	./protocols/OneTryProtocol.cpp


# Coding-related components
lib_rf_codes_la_SOURCES = \
	./codes/EncoderMultiplexer.cpp \
	./codes/puncturing/RepeatingPuncturingSchedule.cpp \
	./codes/puncturing/StaticPuncturingSchedule.cpp \
	./codes/puncturing/RoundRobinPuncturingSchedule.cpp \
	./codes/puncturing/StridedPuncturingSchedule.cpp \
	./codes/RandomPermutationGenerator.cpp \
	./codes/null/NullEncoder.cpp \
	./codes/null/NullDecoder.cpp \
	./codes/InterleavedEncoder.cpp \
	./codes/MultiToSingleStreamEncoder.cpp
lib_rf_fountain_la_SOURCES = \
	./codes/fountain/LTParityNeighborGenerator.cpp \
	./codes/fountain/RaptorDecoder.cpp \
	./codes/fountain/LTDecoder.cpp \
	./codes/fountain/RaptorEncoder.cpp
lib_rf_strider_la_SOURCES = \
	./codes/strider/LayeredEncoder.cpp \
	./codes/strider/StriderFactory.cpp \
	./codes/strider/LayerSuperposition.cpp \
	./codes/strider/StriderGeneratorMatrix.cpp \
	./codes/strider/StriderInterleaver.cpp \
	./codes/strider/LayerManipulator.cpp \
	./codes/strider/StriderTurboCode.cpp
lib_rf_turbo_la_SOURCES = \
	./codes/turbo/TurboEncoder.cpp \
	./codes/turbo/TurboDecoder.cpp \
	./codes/turbo/TurboCodec.cpp
lib_rf_ldpc_la_SOURCES = \
	./codes/ldpc/LinearCheckNodeUpdater.cpp \
	./codes/ldpc/WifiLDPC.cpp \
	./codes/ldpc/MatrixLDPCDecoder.cpp \
	./codes/ldpc/MatrixLDPCEncoder.cpp \
	./codes/ldpc/MatrixLDPCCode.cpp \
	./codes/ldpc/MatrixLDPCNeighborGenerator.cpp


# util libraries
lib_rf_hashes_la_SOURCES = \
	./util/hashes/OneAtATimeHash.cpp \
	./util/hashes/Lookup3Hash.cpp \
	./util/hashes/salsa20.cpp \
	./util/hashes/BitwiseXor.cpp \
	./util/hashes/SalsaHash.cpp 
lib_rf_hashes_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/util/hashes

lib_rf_inference_la_SOURCES = \
	./util/inference/bp/LinearVariableNodeUpdater.cpp \
	./util/inference/bp/BipartiteBP.cpp 
lib_rf_inference_la_CXXFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/util/inference
