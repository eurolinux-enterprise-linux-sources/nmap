
/***************************************************************************
 * IPv6Header.cc -- The IPv6Header Class represents an IPv4 datagram. It   *
 * contains methods to set any header field. In general, these methods do  *
 * error checkings and byte order conversion.                              *
 *                                                                         *
 ***********************IMPORTANT NMAP LICENSE TERMS************************
 *                                                                         *
 * The Nmap Security Scanner is (C) 1996-2011 Insecure.Com LLC. Nmap is    *
 * also a registered trademark of Insecure.Com LLC.  This program is free  *
 * software; you may redistribute and/or modify it under the terms of the  *
 * GNU General Public License as published by the Free Software            *
 * Foundation; Version 2 with the clarifications and exceptions described  *
 * below.  This guarantees your right to use, modify, and redistribute     *
 * this software under certain conditions.  If you wish to embed Nmap      *
 * technology into proprietary software, we sell alternative licenses      *
 * (contact sales@insecure.com).  Dozens of software vendors already       *
 * license Nmap technology such as host discovery, port scanning, OS       *
 * detection, and version detection.                                       *
 *                                                                         *
 * Note that the GPL places important restrictions on "derived works", yet *
 * it does not provide a detailed definition of that term.  To avoid       *
 * misunderstandings, we consider an application to constitute a           *
 * "derivative work" for the purpose of this license if it does any of the *
 * following:                                                              *
 * o Integrates source code from Nmap                                      *
 * o Reads or includes Nmap copyrighted data files, such as                *
 *   nmap-os-db or nmap-service-probes.                                    *
 * o Executes Nmap and parses the results (as opposed to typical shell or  *
 *   execution-menu apps, which simply display raw Nmap output and so are  *
 *   not derivative works.)                                                *
 * o Integrates/includes/aggregates Nmap into a proprietary executable     *
 *   installer, such as those produced by InstallShield.                   *
 * o Links to a library or executes a program that does any of the above   *
 *                                                                         *
 * The term "Nmap" should be taken to also include any portions or derived *
 * works of Nmap.  This list is not exclusive, but is meant to clarify our *
 * interpretation of derived works with some common examples.  Our         *
 * interpretation applies only to Nmap--we don't speak for other people's  *
 * GPL works.                                                              *
 *                                                                         *
 * If you have any questions about the GPL licensing restrictions on using *
 * Nmap in non-GPL works, we would be happy to help.  As mentioned above,  *
 * we also offer alternative license to integrate Nmap into proprietary    *
 * applications and appliances.  These contracts have been sold to dozens  *
 * of software vendors, and generally include a perpetual license as well  *
 * as providing for priority support and updates as well as helping to     *
 * fund the continued development of Nmap technology.  Please email        *
 * sales@insecure.com for further information.                             *
 *                                                                         *
 * As a special exception to the GPL terms, Insecure.Com LLC grants        *
 * permission to link the code of this program with any version of the     *
 * OpenSSL library which is distributed under a license identical to that  *
 * listed in the included docs/licenses/OpenSSL.txt file, and distribute   *
 * linked combinations including the two. You must obey the GNU GPL in all *
 * respects for all of the code used other than OpenSSL.  If you modify    *
 * this file, you may extend this exception to your version of the file,   *
 * but you are not obligated to do so.                                     *
 *                                                                         *
 * If you received these files with a written license agreement or         *
 * contract stating terms other than the terms above, then that            *
 * alternative license agreement takes precedence over these comments.     *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes (none     *
 * have been found so far).                                                *
 *                                                                         *
 * Source code also allows you to port Nmap to new platforms, fix bugs,    *
 * and add new features.  You are highly encouraged to send your changes   *
 * to nmap-dev@insecure.org for possible incorporation into the main       *
 * distribution.  By sending these changes to Fyodor or one of the         *
 * Insecure.Org development mailing lists, it is assumed that you are      *
 * offering the Nmap Project (Insecure.Com LLC) the unlimited,             *
 * non-exclusive right to reuse, modify, and relicense the code.  Nmap     *
 * will always be available Open Source, but this is important because the *
 * inability to relicense code has caused devastating problems for other   *
 * Free Software projects (such as KDE and NASM).  We also occasionally    *
 * relicense the code to third parties as discussed above.  If you wish to *
 * specify special license conditions of your contributions, just say so   *
 * when you send them.                                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * General Public License v2.0 for more details at                         *
 * http://www.gnu.org/licenses/gpl-2.0.html , or in the COPYING file       *
 * included with Nmap.                                                     *
 *                                                                         *
 ***************************************************************************/

#include "IPv6Header.h"
#include "nping.h"
#include "common.h"
#include "dnet.h"
#include "utils.h"


IPv6Header::IPv6Header() {
  this->reset();
} /* End of IPv6Header constructor */


IPv6Header::~IPv6Header() {

} /* End of IPv6Header destructor */


/** Sets every class attribute to zero */
void IPv6Header::reset(){
  memset(&this->h, 0, sizeof(nping_ipv6_hdr_t));
  this->length=40;
} /* End of reset() */


/** @warning This method is essential for the superclass getBinaryBuffer()
 *  method to work. Do NOT change a thing unless you know what you're doing  */
u8 *IPv6Header::getBufferPointer(){
  return (u8*)(&h);
} /* End of getBufferPointer() */


/** Stores supplied packet in the internal buffer so the information
  * can be accessed using the standard get & set methods.
  * @warning  The IPv6Header class is able to hold a maximum of 40 bytes. If the
  * supplied buffer is longer than that, only the first 40 bytes will be stored
  * in the internal buffer.
  * @warning Supplied len MUST be at least 40 bytes (IPv6 header length).
  * @return OP_SUCCESS on success and OP_FAILURE in case of error */
int IPv6Header::storeRecvData(const u8 *buf, size_t len){ 
  if(buf==NULL || len<IPv6_HEADER_LEN){
    return OP_FAILURE;
  }else{
    this->reset(); /* Re-init the object, just in case the caller had used it already */
    this->length=IPv6_HEADER_LEN;
    memcpy(&(this->h), buf, IPv6_HEADER_LEN);
  }
 return OP_SUCCESS;
} /* End of storeRecvData() */


/** This method is provided for consistency with other classes of the
  * PacketElement family. 99.9% of the cases, it returns 40 (the length of the
  * IPv6 header). If for some reason, the internal state of the object is not
  * correct, OP_FAILURE (-1) is returned. */
int IPv6Header::validate(){
  if( this->length!=IPv6_HEADER_LEN)
      return OP_FAILURE;
  else
      return IPv6_HEADER_LEN;
} /* End of validate() */



/** Set Version field (4 bits).  */
int IPv6Header::setVersion(u8 val){
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 ver:4;
            u8 tclass:4;
        #else
            u8 tclass:4;
            u8 ver:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header1stbyte;

  header1stbyte.fullbyte = h.ip6_start[0];
  header1stbyte.halfbyte.ver=val;
  h.ip6_start[0]=header1stbyte.fullbyte;
  return OP_SUCCESS;
} /* End of setVersion() */


/** Set Version field to value 6.  */
int IPv6Header::setVersion(){
  this->setVersion(6);
  return OP_SUCCESS;
} /* End of setVersion() */


/** Returns an 8bit number containing the value of the Version field.  */
u8 IPv6Header::getVersion(){    
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 ver:4;
            u8 tclass:4;
        #else
            u8 tclass:4;
            u8 ver:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header1stbyte;

  header1stbyte.fullbyte = h.ip6_start[0];  
  return (u8)header1stbyte.halfbyte.ver;  
} /* End of getVersion() */


int IPv6Header::setTrafficClass(u8 val){
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 ver:4;
            u8 tclass1:4;
        #else
            u8 tclass1:4;
            u8 ver:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header1stbyte;
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 tclass2:4;
            u8 flow:4;
        #else
            u8 flow:4;
            u8 tclass2:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header2ndbyte;

  /* Store old contents */
  header1stbyte.fullbyte = h.ip6_start[0];
  header2ndbyte.fullbyte = h.ip6_start[1];

  /* Fill the two 4bit halves */
  header1stbyte.halfbyte.tclass1=val>>4;
  header2ndbyte.halfbyte.tclass2=val;

  /* Write the bytes back to the header */
  h.ip6_start[0]=header1stbyte.fullbyte;
  h.ip6_start[1]=header2ndbyte.fullbyte;
  
  return OP_SUCCESS;
} /* End of setTrafficClass() */


u8 IPv6Header::getTrafficClass(){
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 ver:4;
            u8 tclass1:4;
        #else
            u8 tclass1:4;
            u8 ver:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header1stbyte;
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 tclass2:4;
            u8 flow:4;
        #else
            u8 flow:4;
            u8 tclass2:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header2ndbyte;
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 tclass1:4;
            u8 tclass2:4;
        #else
            u8 tclass2:4;
            u8 tclass1:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }finalbyte;

  header1stbyte.fullbyte = h.ip6_start[0];
  header2ndbyte.fullbyte = h.ip6_start[1];
  finalbyte.halfbyte.tclass1=header1stbyte.halfbyte.tclass1;
  finalbyte.halfbyte.tclass2=header2ndbyte.halfbyte.tclass2;
  return finalbyte.fullbyte;
} /* End of getTrafficClass() */


int IPv6Header::setFlowLabel(u32 val){
  u32 netbyte = htonl(val);
  u8 *pnt=(u8*)&netbyte;
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 tclass2:4;
            u8 flow:4;
        #else
            u8 flow:4;
            u8 tclass2:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header2ndbyte;

  header2ndbyte.fullbyte = h.ip6_start[1];
  header2ndbyte.halfbyte.flow=pnt[1];
  h.ip6_start[1]=header2ndbyte.fullbyte;
  h.ip6_start[2]=pnt[2];
  h.ip6_start[3]=pnt[3];
  return OP_SUCCESS;
} /* End of setFlowLabel() */


u32 IPv6Header::getFlowLabel(){
  u32 hostbyte=0;
  u8 *pnt=(u8*)&hostbyte;
  union{
    struct firstbyte{
        #if WORDS_BIGENDIAN
            u8 tclass2:4;
            u8 flow:4;
        #else
            u8 flow:4;
            u8 tclass2:4;
        #endif
    }halfbyte;
    u8 fullbyte;
  }header2ndbyte;

  header2ndbyte.fullbyte = h.ip6_start[1];
  pnt[0]=0;
  pnt[1]=header2ndbyte.halfbyte.flow;
  pnt[2]=h.ip6_start[2];
  pnt[3]=h.ip6_start[3];
  hostbyte=ntohl(hostbyte);
  return hostbyte;
} /* End of getFlowLabel() */


int IPv6Header::setPayloadLength(u16 val){
  this->h.ip6_len = htons(val);
  return OP_SUCCESS;
} /* End of setPayloadLength() */


int IPv6Header::setPayloadLength(){
  int otherslen=0;
  if (next!=NULL)
      otherslen=next->getLen();
  setPayloadLength( otherslen );
  return OP_SUCCESS;
} /* End of setTotalLength() */


u16 IPv6Header::getPayloadLength(){
  return ntohs(this->h.ip6_len);
} /* End of getPayloadLength() */


int IPv6Header::setNextHeader(u8 val){
  this->h.ip6_nh = val;
  return OP_SUCCESS;
} /* End of setNextHeader() */


u8 IPv6Header::getNextHeader(){
  return this->h.ip6_nh;
} /* End of getNextHeader() */


/** Sets field "next header" to the number that corresponds to the supplied
 *  protocol name. Currently onyl TCP, UDP and ICMP are supported. Any
 *  help to extend this functionality would be appreciated. For a list of all
 *  proto names and numbers check:
 *  http://www.iana.org/assignments/protocol-numbers/                        */
int IPv6Header::setNextHeader(const char *p){

  if (p==NULL){
    printf("setNextProto(): NULL pointer supplied\n");
    return OP_FAILURE;
  }
  if( !strcasecmp(p, "TCP") )
    setNextHeader(6);   /* 6=IANA number for proto TCP */
  else if( !strcasecmp(p, "UDP") )
    setNextHeader(17);  /* 17=IANA number for proto UDP */
  else if( !strcasecmp(p, "ICMP") )
    setNextHeader(1);   /* 1=IANA number for proto ICMP */
  else
    outFatal(QT_3, "setNextProto(): Invalid protocol number\n");
  return OP_SUCCESS;  
} /* End of setNextHeader() */


int IPv6Header::setHopLimit(u8 val){
  this->h.ip6_hopl = val;
  return OP_SUCCESS;
} /* End of setHopLimit() */


u8 IPv6Header::getHopLimit(){
  return this->h.ip6_hopl;
} /* End of getHopLimit() */


int IPv6Header::setSourceAddress(u8 *val){
  if(val==NULL)
    outFatal(QT_3,"setSourceAddress(): NULL value supplied.");
  memcpy(this->h.ip6_src, val, 16);
  return OP_SUCCESS;
} /* End of setSourceAddress() */

int IPv6Header::setSourceAddress(struct in6_addr val){
  memcpy(this->h.ip6_src, val.s6_addr, 16);
  return OP_SUCCESS;
} /* End of setSourceAddress() */


u8 *IPv6Header::getSourceAddress(){
  return this->h.ip6_src;
} /* End of getSourceAddress() */


int IPv6Header::setDestinationAddress(u8 *val){
  if(val==NULL)
    outFatal(QT_3,"setDestinationAddress(): NULL value supplied.");        
  memcpy(this->h.ip6_dst, val, 16);
  return OP_SUCCESS;
} /* End of setDestinationAddress() */


u8 *IPv6Header::getDestinationAddress(){
  return this->h.ip6_dst;
} /* End of getDestinationAddress() */
