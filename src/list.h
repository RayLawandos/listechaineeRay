/*
 * list.h
 *
 * Définition des structure de données pour la gestion de liste chainée
 */

#pragma once

typedef struct listchainee_st *listchainee_ptr;

struct listechainee_st {
  int N;
  listchainee_ptr next;
};
