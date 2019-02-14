#include <complex.h>
#include <limits.h>
#include <stddef.h>
#include <stdbool.h>
#include "v2d.h"

#define cmp(a, b) ((a > b) - (a < b))

static size_t _count_actions(v2d_action_t *actions) {
	size_t n = 0;
	while (actions->id) n++, actions++;
	return n;
}

// static int _compare_action(const void *ap, const void *bp) {
// 	const v2d_action_t *a = ap, *b = bp;
// 	return strcmp(a->id, b->id);
// }

// static int _compare_action_key(const void *kp, const void *ap) {
// 	const char *key = kp;
// 	const v2d_action_t *a = ap;
// 	return strcmp(key, a->id);
// }

static size_t _count_triggers(struct v2d_action_trigger *triggers) {
	size_t n = 0;
	while (triggers->type) n++, triggers++;
	return n;
}

static uint64_t _hash_trigger(const struct v2d_action_trigger *trig) {
	// The highest byte should be the trigger type
	uint64_t hash = (uint64_t)trig->type << (64-8);

	// Subsequent bytes depend on the type of trigger
	switch (trig->type) {
	case V2D_ACTION_TRIG_KEY:
		hash |= trig->trig.key.sym << 16;
		hash |= trig->trig.key.mod;
		break;

	case V2D_ACTION_TRIG_MOUSE:
		hash |= trig->trig.mouse.button << 8;
		hash |= trig->trig.mouse.clicks;

	case V2D_ACTION_TRIG_CONTROLLER:
		hash |= trig->trig.controller.button;

	default:
		return -1;
	}
	
	return hash;
}

static int _compare_trigger(const void *ap, const void *bp) {
	const struct v2d_action_trigger *a = ap, *b = bp;
	uint64_t ah = _hash_trigger(a), bh = _hash_trigger(b);
	return cmp(ah, bh);
}

static int _compare_trigger_key(const void *kp, const void *tp) {
	const uint64_t *key = kp;
	const struct v2d_action_trigger *t = tp;
	uint64_t th = _hash_trigger(t);
	return cmp(*key, th);
}

static size_t _count_axes(struct v2d_action_axis *axes) {
	size_t n = 0;
	while (axes->axis != -1) n++, axes++;
	return n;
}

static int _compare_axis(const void *ap, const void *bp) {
	const struct v2d_action_axis *a = ap, *b = bp;
	return cmp(a->axis, b->axis);
}

static int _compare_axis_key(const void *kp, const void *ap) {
	const uint8_t *key = kp;
	const struct v2d_action_axis *a = ap;
	return cmp(*key, a->axis);
}

v2d_action_dispatcher_t v2d_adis_create(struct v2d_action *actions, struct v2d_action_trigger *triggers, struct v2d_action_axis *axes, v2d_action_t *mouse) {
	size_t n_actions = 0, n_triggers = 0, n_axes = 0;

	if (actions) {
		n_actions = _count_actions(actions);
		// Moving these around breaks stuff
		// qsort(actions, n_actions, sizeof *actions, _compare_action);
	}
	
	if (triggers) {
		n_triggers = _count_triggers(triggers);
		qsort(triggers, n_triggers, sizeof *triggers, _compare_trigger);
	}

	if (axes) {
		n_axes = _count_axes(axes);
		qsort(axes, n_axes, sizeof *axes, _compare_axis);
	}

	return (v2d_action_dispatcher_t){
		actions, n_actions,
		triggers, n_triggers,
		axes, n_axes,
		mouse,
	};
}

v2d_action_t *v2d_adis_find_action(v2d_action_dispatcher_t dis, const char *id) {
	for (size_t i = 0; i < dis.n_actions; i++) {
		if (!strcmp(dis.actions[i].id, id)) return dis.actions+i;
	}
	return NULL;
	// if (!dis.actions) return NULL;
	// return bsearch(id, dis.actions, dis.n_actions, sizeof *dis.actions, _compare_action_key);
}

v2d_action_t *v2d_adis_find_trigger_action(v2d_action_dispatcher_t dis, struct v2d_action_trigger trig) {
	if (!dis.triggers) return NULL;

	uint64_t trig_hash = _hash_trigger(&trig);
	struct v2d_action_trigger *found = bsearch(&trig_hash, dis.triggers, dis.n_triggers, sizeof *dis.triggers, _compare_trigger_key);

	if (!found) return NULL;
	return found->action;
}

struct v2d_action_axis *v2d_adis_find_axis(v2d_action_dispatcher_t dis, uint8_t axis) {
	if (!dis.axes) return NULL;
	return bsearch(&axis, dis.axes, dis.n_axes, sizeof *dis.axes, _compare_axis_key);
}

static void _handle_trigger(v2d_action_dispatcher_t dis, struct v2d_action_trigger trig, double active) {
	v2d_action_t *act = v2d_adis_find_trigger_action(dis, trig);
	if (act) act->value.s = active;
}

static void _handle_axis(v2d_action_dispatcher_t dis, uint8_t axis, int16_t val) {
	struct v2d_action_axis *a = v2d_adis_find_axis(dis, axis);
	if (!a || !a->action) return;
	double v = (double)val / (double)INT16_MAX;
	if (-a->dead < v && v < a->dead) v = 0;
	a->action->value.xy[a->xy] = v;
}

void v2d_adis_handle_event(v2d_action_dispatcher_t dis, SDL_Event ev, v2d_transform_t mouse_tr) {
	if (!dis.actions) return;

	v2d_vec_t val = 0;

	switch (ev.type) {
	// --- Triggers ---

	case SDL_KEYDOWN:
		val = 1;
		// fallthrough
	case SDL_KEYUP:
		// We remove KMOD_CAPS and KMOD_NUM because those don't generally matter
		_handle_trigger(dis, V2D_KEY_TRIG_LIT(ev.key.keysym.sym, ev.key.keysym.mod & ~(KMOD_CAPS | KMOD_NUM), NULL), val);
		break;

	case SDL_MOUSEBUTTONDOWN:
		val = 1;
		// fallthrough
	case SDL_MOUSEBUTTONUP:
		_handle_trigger(dis, V2D_MOUSE_TRIG_LIT(ev.button.button, ev.button.clicks, NULL), val);
		break;

	case SDL_CONTROLLERBUTTONDOWN:
		val = 1;
		// fallthrough
	case SDL_CONTROLLERBUTTONUP:
		_handle_trigger(dis, V2D_CONTRL_TRIG_LIT(ev.cbutton.button, NULL), val);
		break;

	// --- Mouse ---

	case SDL_MOUSEMOTION:
		if (!dis.mouse) break;
		val = v2d_vec(ev.motion.x, ev.motion.y);
		// This is an exact inverse of the calculation performed by the v2d_render_draw_* functions to convert game coordinates to screen coordinates
		val = conj(v2d_transform(val, v2d_tr_invert(mouse_tr)));
		dis.mouse->value.pos = val;
		break;

	// --- Axes ---

	case SDL_CONTROLLERAXISMOTION:
		_handle_axis(dis, ev.caxis.axis, ev.caxis.value);
		break;
	}
}
