# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

from factories import *

##
# \ingroup simulator
# \brief Gets a list of factories to query for components
def get_factory_list():
    """
    Returns all the factories that the simulator will be using.
    """
    return [ChannelFactory(), DemapFactory(), DetectorFactory(), MapFactory(), 
            PacketGenFactory(), ProtocolFactory(), StatisticsFactory(),
            codes.FadingStriderFactory(), codes.LdpcFactory(), codes.LTFactory(), 
            codes.MultiplexedFactory(), codes.NullFactory(), codes.RaptorFactory(),
            codes.SpinalFactory(), codes.StriderFactory(), codes.TurboFactory()]
