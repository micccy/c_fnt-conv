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
