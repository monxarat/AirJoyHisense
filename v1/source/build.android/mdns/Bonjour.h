/**
 *
 * bonjour.h
 * 
 * jxfengzi@gmail.com
 *
 * @date   2011-7-31
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __BONJOUR_H__
#define __BONJOUR_H__

#ifdef __cplusplus
extern "C" {
#endif


int mdnsd_start(int debug);
int mdnsd_stop(void);


#ifdef __cplusplus
}
#endif

#endif /* __BONJOUR_H__ */
