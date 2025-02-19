/*****************************************************************
 * gmerlin-avdecoder - a general purpose multimedia decoding library
 *
 * Copyright (c) 2001 - 2012 Members of the Gmerlin project
 * gmerlin-general@lists.sourceforge.net
 * http://gmerlin.sourceforge.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * *****************************************************************/

#include <stdlib.h>
#include <string.h>

#include <avdec_private.h>


bgav_packet_t * bgav_packet_create()
  {
  bgav_packet_t * ret = calloc(1, sizeof(*ret));
  return ret;
  }

void bgav_packet_free(bgav_packet_t * p)
  {
  gavl_buffer_free(&p->buf);
  
  memset(p, 0, sizeof(*p));
  }

void bgav_packet_destroy(bgav_packet_t * p)
  {
  bgav_packet_free(p);
  free(p);
  }

void bgav_packet_alloc(bgav_packet_t * p, int size)
  {
  gavl_buffer_alloc(&p->buf, size + GAVL_PACKET_PADDING);
  
  /* Pad in advance */
  memset(p->buf.buf + size, 0, GAVL_PACKET_PADDING);
  }

void bgav_packet_pad(bgav_packet_t * p)
  {
  /* Padding */
  memset(p->buf.buf + p->buf.len, 0, GAVL_PACKET_PADDING);
  }

#if 0
void bgav_packet_dump(const bgav_packet_t * p)
  {
  bgav_dprintf("pos: %"PRId64", K: %d, ", p->position, !!PACKET_GET_KEYFRAME(p));

  if(p->field2_offset)
    bgav_dprintf("f2: %d, ", p->field2_offset);
  
  bgav_dprintf("T: %s ", bgav_coding_type_to_string(p->flags));
  
  if(p->dts != GAVL_TIME_UNDEFINED)
    bgav_dprintf("dts: %"PRId64", ", p->dts);

  if(p->pts == GAVL_TIME_UNDEFINED)
    bgav_dprintf("pts: (none), ");
  else
    bgav_dprintf("pts: %"PRId64", ", p->pts);
  
  bgav_dprintf("Len: %d, dur: %"PRId64, p->buf.len, p->duration);

  if(p->header_size)
    bgav_dprintf(", head: %d", p->header_size);

  if(p->sequence_end_pos)
    bgav_dprintf(", end: %d", p->sequence_end_pos);

  if(PACKET_GET_SKIP(p))
    bgav_dprintf(", skip");

  if(p->flags & GAVL_PACKET_NOOUTPUT)
    bgav_dprintf(", nooutput");

  if(p->flags & GAVL_PACKET_REF)
    bgav_dprintf(", ref");

  if(p->flags & GAVL_PACKET_FIELD_PIC)
    bgav_dprintf(", field-pic");
  
  if(p->timecode != GAVL_TIMECODE_UNDEFINED)
    {
    bgav_dprintf(", TC: ");
    gavl_timecode_dump(NULL, p->timecode);
    }
  
  if(p->interlace_mode == GAVL_INTERLACE_TOP_FIRST)
    bgav_dprintf(", il: t");
  else if(p->interlace_mode == GAVL_INTERLACE_BOTTOM_FIRST)
    bgav_dprintf(", il: b");

  //  if(p->end_pts != GAVL_TIME_UNDEFINED)
  //    bgav_dprintf(" end_pts: %"PRId64", ", p->end_pts);
  
  bgav_dprintf("\n");
  //  gavl_hexdump(p->data, p->data_size < 16 ? p->data_size : 16, 16);
  }
#endif

void bgav_packet_dump_data(bgav_packet_t * p, int bytes)
  {
  if(bytes > p->buf.len)
    bytes = p->buf.len;
  gavl_hexdump(p->buf.buf, bytes, 16);
  }

void bgav_packet_copy_metadata(bgav_packet_t * dst,
                               const bgav_packet_t * src)
  {
  dst->pts      = src->pts;
  dst->dts      = src->dts;
  dst->duration = src->duration;
  dst->flags    = src->flags;
  dst->timecode       = src->timecode;
  }

void bgav_packet_copy(bgav_packet_t * dst,
                      const bgav_packet_t * src)
  {
  memcpy(dst, src, sizeof(*dst));
  memset(&dst->buf, 0, sizeof(dst->buf));
  gavl_buffer_copy(&dst->buf, &src->buf);
  }

#if 0
void bgav_packet_source_copy(bgav_packet_source_t * dst,
                             const bgav_packet_source_t * src)
  {
  memcpy(dst, src, sizeof(*dst));
  }

void bgav_packet_2_gavl(bgav_packet_t * src,
                        gavl_packet_t * dst)
  {
  dst->pts      = src->pts;
  dst->duration = src->duration;

  dst->header_size      = src->header_size;
  dst->field2_offset    = src->field2_offset;
  dst->sequence_end_pos = src->sequence_end_pos;

  dst->flags    = src->flags & 0xFFFF;

  dst->buf.buf = src->buf.buf;
  dst->buf.len = src->buf.len;
  dst->buf.alloc = src->buf.alloc;
  
  dst->interlace_mode = src->interlace_mode;
  dst->timecode = src->timecode;

  dst->dst_x = src->dst_x;
  dst->dst_y = src->dst_y;

  gavl_rectangle_i_copy(&dst->src_rect, &src->src_rect);
  }

void bgav_packet_from_gavl(gavl_packet_t * src,
                           bgav_packet_t * dst)
  {
  dst->pts      = src->pts;
  dst->duration = src->duration;
  dst->flags    = src->flags;

  dst->header_size      = src->header_size;
  dst->field2_offset    = src->field2_offset;
  dst->sequence_end_pos = src->sequence_end_pos;
  dst->interlace_mode = src->interlace_mode;
  dst->timecode = src->timecode;

  dst->dst_x = src->dst_x;
  dst->dst_y = src->dst_y;

  gavl_rectangle_i_copy(&dst->src_rect, &src->src_rect);
  }
#endif
