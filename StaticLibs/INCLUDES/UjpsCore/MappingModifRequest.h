#ifndef MAPPING_MODIF_REQUEST
#define MAPPING_MODIF_REQUEST


class AbstractMapping;
class AbstractRealJoystick;
using uint = unsigned int;


enum class MappingModifRequestType
{
	RequestUnmapAll,
	RequestUnmapButton,
	RequestUnmapAxis,
	RequestUnmapPov,
	RequestUnmapRexec,
	RequestAddMapping
};


struct MappingModifRequest
{
	MappingModifRequestType type;
	AbstractRealJoystick *rj;
	uint rnum;
	AbstractMapping *mapping;
};


#endif

