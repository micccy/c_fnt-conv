struct __attribute((__packed__)) psf1h_def{
	uint16_t magicid;
	uint8_t flags;
	uint8_t size;
};

struct __attribute((__packed__)) psf2h_def{
	uint32_t magicid;
	uint32_t version;
	uint32_t hsize;
	uint32_t flags;
	uint32_t length;
	uint32_t csize;
	uint32_t height;
	uint32_t width;
};

struct __attribute((__packed__)) cpiffh_def{	// Font File Header (main)
	uint8_t id[8];								//0xFF,"FONT   "
	uint8_t reserved[8];
	uint16_t pnum;
	uint8_t ptyp;
	uint32_t fihoff;							//Offset to Font Info Header
};

struct __attribute((__packed__)) cpiceh_def{	// Codepage Entry Header
	uint16_t size;								//Size of this header (26 or 28, inconsistent, watch out)
	uint32_t next;								//Next codepage entry, 0 or -1 for last
	uint16_t device;							//1:screen, 2:printer
	uint8_t devname[8];							//Device name
	uint16_t cpid;								//Codepage number
	uint8_t reserved[6];
	uint32_t cih;								//Might only be uint16_t. Offset to Codepage Info header
};

struct __attribute((__packed__)) cpifih_def{	// Font Info Header
	uint16_t cpnum;								//Number of codepages
	struct cpiceh_def fcp;						//First codepage entry
};
