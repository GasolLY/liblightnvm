/*
 * be - Provides fall-back methods and helper functions for actual backends
 *
 * Copyright (C) 2015-2017 Javier Gonzáles <javier@cnexlabs.com>
 * Copyright (C) 2015-2017 Matias Bjørling <matias@cnexlabs.com>
 * Copyright (C) 2015-2017 Simon A. F. Lund <slund@cnexlabs.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <liblightnvm.h>
#include <nvm_be.h>
#include <nvm_dev.h>

static struct nvm_be *nvm_be_imps[] = {
	&nvm_be_ioctl,
	&nvm_be_lbd,
	&nvm_be_spdk,
	&nvm_be_nocd,
	NULL
};

/**
 * Resolve the identifier of the given backend name.
 *
 * @returns Returns numerical identifier on success. On error, e.g. the given
 * name does not exists in the list of backend implementations, -1 is returned.
 */
static inline int nvm_be_name2id(const char *bname)
{
	for (int i = 0; nvm_be_imps[i]; ++i) {
		if (strcasecmp(bname, nvm_be_imps[i]->name)) {
			continue;
		}
		
		NVM_DEBUG("INFO: '%s' -> '%d'", bname, nvm_be_imps[i]->id);
		return nvm_be_imps[i]->id;
	}

	NVM_DEBUG("FAILED: no backend with name: '%s'", bname);
	return -1;
}

static inline const char *nvm_be_id2name(enum nvm_be_id bid)
{
	for (int i = 0; nvm_be_imps[i]; ++i) {
		if (nvm_be_imps[i]->id != bid) {
			continue;
		}
		
		return nvm_be_imps[i]->name;
	}

	NVM_DEBUG("FAILED: no backend with id: '%d'", bid);
	return "";
}

static inline uint64_t _ilog2(uint64_t x)
{
	uint64_t val = 0;

	while (x >>= 1)
		val++;

	return val;
}

static inline void construct_ppaf_mask(struct nvm_spec_ppaf_nand *ppaf,
					struct nvm_spec_ppaf_nand_mask *mask)
{
	for (int i = 0 ; i < 12; ++i) {
		if ((i % 2)) {
			// i-1 = offset
			// i = width
			mask->a[i/2] = (((uint64_t)1<< ppaf->a[i])-1) << ppaf->a[i-1];
		}
	}
}

struct nvm_dev* nvm_be_nosys_open(const char *NVM_UNUSED(dev_path),
				  int NVM_UNUSED(flags))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return NULL;
}

void nvm_be_nosys_close(struct nvm_dev *NVM_UNUSED(dev))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return;
}

int nvm_be_nosys_pass(struct nvm_dev *NVM_UNUSED(dev),
		      struct nvm_nvme_cmd *NVM_UNUSED(cmd),
		      void *NVM_UNUSED(data), size_t NVM_UNUSED(data_nbytes),
		      void *NVM_UNUSED(meta), size_t NVM_UNUSED(meta_nbytes),
		      int NVM_UNUSED(flags), struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

struct nvm_spec_idfy *nvm_be_nosys_idfy(struct nvm_dev *NVM_UNUSED(dev),
					struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return NULL;
}

struct nvm_spec_rprt *nvm_be_nosys_rprt(struct nvm_dev *NVM_UNUSED(dev),
					struct nvm_addr *NVM_UNUSED(addr),
					int NVM_UNUSED(opt),
				        struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return NULL;
}

int nvm_be_nosys_gfeat(struct nvm_dev *NVM_UNUSED(dev), uint8_t NVM_UNUSED(id),
		       union nvm_nvme_feat *NVM_UNUSED(feat),
		       struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented (possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_sfeat(struct nvm_dev *NVM_UNUSED(dev), uint8_t NVM_UNUSED(id),
		       const union nvm_nvme_feat *NVM_UNUSED(feat),
		       struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented (possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

struct nvm_spec_bbt *nvm_be_nosys_gbbt(struct nvm_dev *NVM_UNUSED(dev),
				       struct nvm_addr NVM_UNUSED(addr),
				       struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return NULL;
}

int nvm_be_nosys_sbbt(struct nvm_dev *NVM_UNUSED(dev),
		      struct nvm_addr *NVM_UNUSED(addrs),
		      int NVM_UNUSED(naddrs), uint16_t NVM_UNUSED(flags),
		      struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_scalar_erase(struct nvm_dev *NVM_UNUSED(dev),
			      struct nvm_addr *NVM_UNUSED(addrs),
			      int NVM_UNUSED(naddrs),
			      uint16_t NVM_UNUSED(flags),
			      struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_scalar_write(struct nvm_dev *NVM_UNUSED(dev),
			      struct nvm_addr NVM_UNUSED(addrs),
			      int NVM_UNUSED(naddrs),
			      const void *NVM_UNUSED(data),
			      const void *NVM_UNUSED(meta),
			      uint16_t NVM_UNUSED(flags),
			      struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_scalar_read(struct nvm_dev *NVM_UNUSED(dev),
			     struct nvm_addr NVM_UNUSED(addrs),
			     int NVM_UNUSED(naddrs), void *NVM_UNUSED(data),
			     void *NVM_UNUSED(meta), uint16_t NVM_UNUSED(flags),
			     struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_vector_erase(struct nvm_dev *NVM_UNUSED(dev),
			      struct nvm_addr *NVM_UNUSED(addrs),
			      int NVM_UNUSED(naddrs),
			      void *NVM_UNUSED(meta),
			      uint16_t NVM_UNUSED(flags),
			      struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_vector_write(struct nvm_dev *NVM_UNUSED(dev),
			      struct nvm_addr *NVM_UNUSED(addrs),
			      int NVM_UNUSED(naddrs),
			      const void *NVM_UNUSED(data),
			      const void *NVM_UNUSED(meta),
			      uint16_t NVM_UNUSED(flags),
			      struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_vector_read(struct nvm_dev *NVM_UNUSED(dev),
			     struct nvm_addr *NVM_UNUSED(addrs),
			     int NVM_UNUSED(naddrs),
			     void *NVM_UNUSED(data),
			     void *NVM_UNUSED(meta),
			     uint16_t NVM_UNUSED(flags),
			     struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_vector_copy(struct nvm_dev *NVM_UNUSED(dev),
			     struct nvm_addr NVM_UNUSED(src[]),
			     struct nvm_addr NVM_UNUSED(dst[]),
			     int NVM_UNUSED(naddrs),
			     uint16_t NVM_UNUSED(flags),
			     struct nvm_ret *NVM_UNUSED(ret))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

struct nvm_async_ctx *nvm_be_nosys_async_init(struct nvm_dev *NVM_UNUSED(dev),
					      uint32_t NVM_UNUSED(depth),
					      uint16_t NVM_UNUSED(flags))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return NULL;
}

int nvm_be_nosys_async_term(struct nvm_dev *NVM_UNUSED(dev),
			    struct nvm_async_ctx *NVM_UNUSED(ctx))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_async_poke(struct nvm_dev *NVM_UNUSED(dev),
			    struct nvm_async_ctx *NVM_UNUSED(ctx),
			    uint32_t NVM_UNUSED(max))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_nosys_async_wait(struct nvm_dev *NVM_UNUSED(dev),
			    struct nvm_async_ctx *NVM_UNUSED(ctx))
{
	NVM_DEBUG("FAILED: not implemented(possibly intentionally)");
	errno = ENOSYS;
	return -1;
}

int nvm_be_split_dpath(const char *dev_path, char *nvme_name, int *nsid)
{
	const char prefix[] = "/dev/nvme";
	const size_t prefix_len = strlen(prefix);
	int val;

	if (strlen(dev_path) < prefix_len + 3) {
		errno = EINVAL;
		return -1;
	}

	if (strncmp(prefix, dev_path, prefix_len)) {
		errno = EINVAL;
		return -1;
	}

	val = atoi(&dev_path[strlen(dev_path)-1]);
	if ((val < 1) || (val > 1024)) {
		errno = EINVAL;
		return -1;
	}

	strncpy(nvme_name, dev_path + 5, 5);
	*nsid = val;

	return 0;
}

int nvm_be_populate_quirks(struct nvm_dev *dev, const char serial[])
{
	const int serial_len = strlen(serial);

	if (!strncmp("CX8800ES", serial, 8 < serial_len ? 8 : serial_len)) {
		dev->quirks |= NVM_QUIRK_PMODE_ERASE_RUNROLL;

		switch(dev->verid) {
		case NVM_SPEC_VERID_12:
			dev->quirks |= NVM_QUIRK_OOB_2LRG;
			break;

		case NVM_SPEC_VERID_20:
			dev->quirks |= NVM_QUIRK_OOB_READ_1ST4BYTES_NULL;
			break;
		}

	} else {
		NVM_DEBUG("INFO: no known quirks for serial: %s", serial);
		return 0;
	}

	// HOTFIX: for reports of unrealisticly large OOB area
	if ((dev->quirks & NVM_QUIRK_OOB_2LRG) &&
	    (dev->geo.g.meta_nbytes > (dev->geo.g.sector_nbytes * 0.1))) {
		dev->geo.g.meta_nbytes = 16; // Naively hope this is right
	}

	return 0;
}

int nvm_be_populate_derived(struct nvm_dev *dev)
{
	struct nvm_geo *geo = &dev->geo;

	switch(dev->verid) {
	case NVM_SPEC_VERID_12:
		geo->g.nsectors = geo->g.page_nbytes / geo->g.sector_nbytes;

		/* Derive total number of bytes on device */
		geo->tbytes = geo->g.nchannels * geo->g.nluns * \
				geo->g.nplanes * geo->g.nblocks * \
				geo->g.npages * geo->g.nsectors * \
				geo->g.sector_nbytes;

		/* Derive the sector-shift-width for LBA mapping */
		dev->ssw = _ilog2(geo->g.sector_nbytes);

		/* Derive a default plane mode */
		switch(geo->nplanes) {
		case 4:
			nvm_dev_set_pmode(dev, NVM_FLAG_PMODE_QUAD);
			break;
		case 2:
			nvm_dev_set_pmode(dev, NVM_FLAG_PMODE_DUAL);
			break;
		case 1:
			nvm_dev_set_pmode(dev, NVM_FLAG_PMODE_SNGL);
			break;

		default:
			NVM_DEBUG("FAILED: invalid geo->nplanes: %lu",
				  geo->nplanes);
			errno = EINVAL;
			return -1;
		}
		break;

	case NVM_SPEC_VERID_20:
		/* Derive total number of bytes on device */
		geo->tbytes = geo->l.npugrp * geo->l.npunit * geo->l.nchunk * \
			      geo->l.nsectr * geo->l.nbytes;

		/* Derive the sector-shift-width for LBA mapping */
		dev->ssw = _ilog2(geo->l.nbytes);

		break;
	}

	nvm_dev_set_erase_naddrs_max(dev, NVM_NADDR_MAX);
	nvm_dev_set_write_naddrs_max(dev, NVM_NADDR_MAX);
	nvm_dev_set_read_naddrs_max(dev, NVM_NADDR_MAX);
	nvm_dev_set_meta_mode(dev, NVM_META_MODE_NONE);

	return 0;
}

int nvm_be_populate(struct nvm_dev *dev, struct nvm_be *be)
{
	struct nvm_geo *geo = &dev->geo;
	struct nvm_spec_idfy *idfy = NULL;
	int err;

	dev->be = be;			// TODO: Clean the init. process

	idfy = be->idfy(dev, NULL);
	if (!idfy) {
		NVM_DEBUG("FAILED: be->idfy(...)");
		return -1;
	}
	
	if (idfy->s.verid == 0x3) {	// Handle IDFY reporting 3, mark as 2.0
		idfy->s.verid = NVM_SPEC_VERID_20;
	}

	dev->idfy = *idfy;
	dev->verid = geo->verid = idfy->s.verid;

	switch (idfy->s.verid) {
	case NVM_SPEC_VERID_12:
		// Geometry
		geo->page_nbytes = idfy->s12.grp[0].fpg_sz;
		geo->sector_nbytes = idfy->s12.grp[0].csecs;
		geo->meta_nbytes = idfy->s12.grp[0].sos;

		geo->nchannels = idfy->s12.grp[0].num_ch;
		geo->nluns = idfy->s12.grp[0].num_lun;
		geo->nplanes = idfy->s12.grp[0].num_pln;
		geo->nblocks = idfy->s12.grp[0].num_blk;
		geo->npages = idfy->s12.grp[0].num_pg;

		// Capabilities
		dev->mccap = idfy->s12.grp[0].mccap;

		// Addressing format / offset / mask
		dev->ppaf = idfy->s12.ppaf;

		construct_ppaf_mask(&dev->ppaf, &dev->mask);
		break;

	case NVM_SPEC_VERID_20:
		// Geometry
		geo->l.npugrp = idfy->s20.lgeo.npugrp;
		geo->l.npunit = idfy->s20.lgeo.npunit;
		geo->l.nchunk = idfy->s20.lgeo.nchunk;
		geo->l.nsectr = idfy->s20.lgeo.nsectr;

		struct nvm_nvme_lbaf lbaf = dev->ns.lbaf[dev->ns.flbas & 0xf];

		geo->l.nbytes = 1 << lbaf.ds;
		geo->l.nbytes_oob = lbaf.ms;

		// Capabilities
		dev->mccap = idfy->s20.mccap;

		// Addressing format / offset / mask
		dev->lbaf = idfy->s20.lbaf;

		dev->lbaz.sectr = 0;
		dev->lbaz.chunk = dev->lbaf.sectr;
		dev->lbaz.punit = dev->lbaz.chunk + dev->lbaf.chunk;
		dev->lbaz.pugrp = dev->lbaz.punit + dev->lbaf.punit;

		dev->lbam.sectr = (((uint64_t)1 << dev->lbaf.sectr) - 1) << dev->lbaz.sectr;
		dev->lbam.chunk = (((uint64_t)1 << dev->lbaf.chunk) - 1) << dev->lbaz.chunk;
		dev->lbam.punit = (((uint64_t)1 << dev->lbaf.punit) - 1) << dev->lbaz.punit;
		dev->lbam.pugrp = (((uint64_t)1 << dev->lbaf.pugrp) - 1) << dev->lbaz.pugrp;
		break;

	default:
		NVM_DEBUG("FAILED: Unsupported Version ID(%d)", idfy->s.verid);
		errno = ENOSYS;
		goto failed;
	}

	err = nvm_be_populate_derived(dev);
	if (err) {
		NVM_DEBUG("FAILED: nvm_be_populate_derived");
		goto failed;
	}
	
	err = nvm_be_populate_quirks(dev, "");
	if (err) {
		NVM_DEBUG("FAILED: nvm_be_populate_quirks");
		goto failed;
	}

	nvm_buf_free(dev, idfy);

	return 0;

failed:
	nvm_buf_free(dev, idfy);
	dev->be = NULL;			// TODO: Clean the init. process
	return -1;
}

struct nvm_dev *nvm_be_factory(const char *dev_ident, int flags)
{
	int bid;

	bid = flags & NVM_BE_ALL;
	if ((bid == NVM_BE_ANY) && (getenv("NVM_BE"))) {
		bid = nvm_be_name2id(getenv("NVM_BE"));
	}

	switch (bid) {
	case NVM_BE_IOCTL:
	case NVM_BE_LBD:
	case NVM_BE_SPDK:
	case NVM_BE_NOCD:
	case NVM_BE_ANY:
		break;

	default:
		NVM_DEBUG("FAILED: invalid bid: '%d'", bid);
		return NULL;
	}

	for (int i = 0; nvm_be_imps[i]; ++i) {
		struct nvm_dev *dev = NULL;

		if (bid && !(nvm_be_imps[i]->id & bid)) {
			continue;
		}

		dev = nvm_be_imps[i]->open(dev_ident, flags);
		if (!dev) {
			continue;
		}

		dev->be = nvm_be_imps[i];
		return dev;
	}

	return NULL;
}

