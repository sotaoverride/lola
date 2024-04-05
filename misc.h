/*
 * misc.h
 *
 *  Created on: Apr 1, 2024
 *      Author: aatir
 */

#ifndef MISC_H_
#define MISC_H_

extern char
Hex (char Character);
extern uint64_t
get_time (void);
extern int
BuildSentence (TGPS *GPS, char *TxLine, const char *PayloadID);

#endif /* MISC_H_ */
