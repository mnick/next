/*
Copyright © 2018 Atlas Engineer LLC.
Use of this file is governed by the license that can be found in LICENSE.
*/
#pragma once

// The keys must be dynamically allocated strings.  They are automatically freed
// when the hash table ref count reaches zero.  The values must be freed
// manually.

#include <glib.h>

typedef struct {
	GHashTable *dict;
	guint element_count;
} AutokeyDictionary;

static void akd_key_destroy_func(gpointer data) {
	g_free(data);
}

AutokeyDictionary *akd_init(GDestroyNotify value_destroy_func) {
	AutokeyDictionary *self = calloc(1, sizeof (AutokeyDictionary));
	self->element_count = 0;
	self->dict = g_hash_table_new_full(g_str_hash, g_str_equal,
			&akd_key_destroy_func, value_destroy_func);
	return self;
}

void akd_free(AutokeyDictionary *self) {
	g_hash_table_unref(self->dict);
	g_free(self);
}

char *akd_insert_element(AutokeyDictionary *self, gpointer object) {
	char *element_key = g_strdup_printf("%i", self->element_count);
	g_hash_table_insert(self->dict, element_key, object);
	self->element_count++;
	return element_key;
}

gpointer akd_object_for_key(AutokeyDictionary *self, const char *a_key) {
	return g_hash_table_lookup(self->dict, a_key);
}

void akd_remove_object_for_key(AutokeyDictionary *self, const char *a_key) {
	// g_hash_table_remove() calls akd_key_destroy_func() to free the key and
	// value_destroy_func (passed in akd_init()) to free the value.
	g_hash_table_remove(self->dict, a_key);
}
