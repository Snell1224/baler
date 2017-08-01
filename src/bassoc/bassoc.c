/* -*- c-basic-offset: 8 -*-
 * Copyright (c) 2015-16 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2015-16 Sandia Corporation. All rights reserved.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the U.S. Government.
 * Export of this program may require a license from the United States
 * Government.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of Sandia nor the names of any contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *      Neither the name of Open Grid Computing nor the names of any
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *      Modified source versions must be plainly marked as such, and
 *      must not be misrepresented as being the original software.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file bassoc.c
 * \brief Baler association rule mining utility
 * \author Narate Taerat (narate at ogc dot us)
 */

/**
 * \page bassoc Baler association rule mining utility
 *
 * \section desc DESCRIPTION
 * This is a Baler's association rule mining utility. It can initiate workspace,
 * extract images information from balerd's store, and mine association rules
 * from the extracted images. Please see \ref example for usage examples.
 *
 * \section synopsis SYNOPSIS
 * create workspace:
 * \par
 * \code
 *     bassoc -c -w WORKSPACE [-t NUMBER] [-n NUMBER]
 * \endcode
 *
 * extract images from baler store:
 * \par
 * \code
 *     bassoc -x -w WORKSPACE -p BSTORE_PLUGIN -s BALERD_STORE [-B TS] [-E TS] \\
 *            [-H IDS] -R RECIPE_FILE
 *
 *     bassoc -x -w WORKSPACE -p BSTORE_PLUGIN -s BALERD_STORE [-B TS] [-E TS] \\
 *            [-H IDS] -r RECIPE1 -r RECIPE2 ...
 * \endcode
 *
 * extract images from metric input stream:
 * \par
 * \code
 *     cat hdr.csv metric.csv | bassoc -X -w WORKSPACE -R RECIPE_FILE
 * \endcode
 *
 * extract from both baler store and metric input stream:
 * \par
 * \code
 *     cat hdr.csv metric.csv | bassoc -X -w WORKSPACE -s BALERD_STORE -R RECIPE_FILE
 * \endcode
 *
 * mine for associations:
 * \par
 * \code
 *     # provide target list via CLI argument
 *     bassoc -w WORKSPACE [-o PIXEL_OFFSET] -m TARGET_LIST
 *
 *     # provide target list via TARGET_FILE
 *     bassoc -w WORKSPACE [-o PIXEL_OFFSET] -M TARGET_FILE
 *
 *     # Use black-white co-occurrence evaluation
 *     bassoc -w WORKSPACE -b [-o PIXEL_OFFSET] -m TARGET_LIST
 *     bassoc -w WORKSPACE -b [-o PIXEL_OFFSET] -M TARGET_FILE
 * \endcode
 *
 * \section options OPTIONS
 *
 * \b NOTE: The order of the parameters are not important.
 *
 * \par -h,--help
 * Show help message.
 *
 * \par -i,--info
 * Show information of the workspace.
 *
 * \par -v,--verbose DEBUG|INFO|WARN|ERROR
 * Verbosity level of the program. The default value is 'INFO'.
 *
 * \par -c,--create
 * Create and initialize workspace (-w). If the workspace existed, the program
 * will exit with error.
 *
 * \par -w,--workspace WORKSPACE_DIR
 * Workspace directory. With '-i', the WORKSPACE_DIR will be created, or the
 * program exits with error if WORKSPACE_DIR existed. Without '-i', the program
 * will exit with error if the workspace does not exist or not in a good state.
 *
 * \par -t,--sec-per-pixel NUMBER
 * The number of seconds in a pixel of the image. This option is used only with
 * create mode ('-c'). If not specified, the default value is 3600.
 *
 * \par -n,--node-per-pixel NUMBER
 * The number of nodes in a pixel of the image. This option is used only with
 * create mode ('-c'). If not specified, the default value is 1.
 *
 * \par -x,--extract
 * This option will make bassoc run in image extraction mode.
 *
 * \par -X,--metric-stream
 * Same as '-x', but with metric input stream enabled.
 *
 * \par -p,--store-plugin BSTORE_PLUGIN
 * BSTORE plugin. This is the plugin name given to the '-S' option of `balerd`.
 * This option is required if '-s' is specified.
 *
 * \par -s,--store STORE_DIR
 * This is a path to balerd's store. This option is needed for image extraction
 * from message occurrences. If it is not given, the \c query options (-B,-E,-H)
 * will be ignored.
 *
 * \par -B,--ts-begin TIME_STAMP
 * The beginning timestamp for the image extraction (-x). TIME_STAMP can be
 * either in 'seconds since Epoch' or 'yyyy-mm-dd HH:MM:SS'. If not specified,
 * the earliest time in the store is used.
 *
 * \par -E,--ts-end TIME_STAMP
 * The ending timestamp for the image extraction (-x). TIME_STAMP can be either
 * in 'seconds since Epoch' or 'yyyy-mm-dd HH:MM:SS'. If not specified, the
 * latest timestamp in the database is used.
 *
 * \par -H,--host-ids HOST_ID_LIST
 * This option is for image extraction (-x). It is a comma separated list of
 * ranges of host IDs (e.g. '2,4,6-10,29'). If not specified, all host IDs will
 * be included.
 *
 * \par -r,--recipe RECIPE
 * Please see \ref recipe below.
 *
 * \par
 * \b NOTE: This is useful if you have only a handful of images to generate. If
 * you have more images to extract, it is advisable to use '-R' option instead.
 *
 * \par -R,--recipe-file IMG_RECIPE_FILE
 * IMG_RECIPE_FILE is a text file, each line of which contains a recipe to
 * create an image. Please see \ref recipe_file for more information.
 *
 * \par -o,--offset NUMBER
 * The number of PIXEL to be offset when comparing the causes to the effect. See
 * \ref offset for more information.
 *
 * \par -b,--black-white
 * A flag to use black-white image evaluation. See \ref blackwhite below.
 *
 * \par -m,--mine-target TARGET_LIST
 * Mine the association rules that have target in the TARGET_LIST. TARGET_LIST
 * is a comma-separated list of the names of the target images.
 *
 * \par -M,--mine-target-file TARGET_FILE
 * Mine the association rules that have target in the TARGET_FILE. The
 * TARGET_FILE is a text file each line of which contains target image name. The
 * line begins with '#' will be ignored.
 *
 * \par -K,--confidence-threshold NUMBER (0.0 - 1.0)
 * Confidence threshold for the miner to accept a rule candidate as a rule.
 * A rule candidate will is a rule if it satisfies both confidence and
 * significance.
 *
 * \par -S,--significance-threshold NUMBER (0.0 - 1.0)
 * Significance threshold for the miner to accept a rule candidate as a rule.
 * A rule candidate will is a rule if it satisfies both confidence and
 * significance.
 *
 * \par -D,--difference-threshold NUMBER (0.0 - 1.0)
 * A threshold to help miner bound the search branch. If the antecedence in the
 * new rule candidate does not differ (in occurrences) to the antecedence in the
 * current active rule candidate, then the search of the new rule candidate
 * branch will be bounded.
 *
 * \section recipe RECIPE
 * There are two kinds of recipes, message pattern recipes and metric bin
 * recipes.
 *
 * \subsection msgptn_recipe MESSAGE PATTERN RECIPE
 * A MESSAGE PATTERN RECIPE is described as 'NAME:PTN_ID_LIST'. This option is
 * used with extract (-x) option. An image, of name NAME will be created with
 * occurrences of patterns specified in PTN_ID_LIST. The PTN_ID_LIST is in the
 * same format as HOST_ID_LIST above. The NAME is [A-Za-z0-9._]+ and must be
 * unique in the workspace. This option is repeatable, i.e. if the following
 * options are given:
 *
 * \par
 * \code
 * -r a:555 -r b:556-570 -r c:600
 * \endcode
 *
 * Three images a, b, and c will be created from pattern ID 555, 556-570 and 600
 * respectively.
 *
 * \subsection metricbin_recipe METRIC BIN RECIPE
 * A METRIC BIN RECIPE is described as '+NAME:BIN_LIST'. It looks very similar
 * to MESSAGE PATTERN RECIPE, only that its name must begin with '+' and the
 * interpretation of numbers after ':' is different. In METRIC BIN RECIPE, the
 * sequence of ascending numbers are the cuts for metric binning. Consider the
 * following recipe:
 *
 * \par
 * \code
 * +MemFree:1e+06,1e+07,1e+08,1e+09
 * \endcode
 *
 * From the above example, \c bassoc will extract the following images:
 * - +MemFree[-Inf,1e+06)
 * - +MemFree[1e+06,1e+07)
 * - +MemFree[1e+07,1e+08)
 * - +MemFree[1e+08,1e+09)
 * - +MemFree[1e+09,Inf)
 *
 * These images contain the count of occurrences (from metric input stream) of
 * the value of MemFree within the ranges in each corresponding image. The x-y
 * coordinates are interpreted as time and space the same way that images
 * generated by message patterns do.
 *
 * METRIC BIN RECIPE can be given in command-line arguments (with the
 * option '-r'), or in recipe file.
 *
 * \section recipe_file RECIPE FILE
 * The format of each line is as following:
 *
 * \par
 * \code
 *     NAME:PTN_ID_LIST
 *     +NAME:BIN_LIST
 *     # COMMENT
 * \endcode
 *
 * The NAME is [A-Za-z0-9._]+ and must be unique in the
 * workspace. If the image of the same name had already existed in the
 * workspace, \c bassoc will exit with error. The line beginning with '#' will
 * be ignored.
 *
 * The PTN_ID_LIST is in the same format as HOST_ID_LIST (list or
 * comma-separated ranges).
 *
 * The '+NAME' format is for Metric values.
 *
 * The BIN_LIST is a comma-separated list of ascending numbers for metric
 * binning.
 *
 * The images will be created according to sec/pixel and node/pixel information
 * in the workspace.
 *
 * \section img IMAGE
 * This section explains about Images that represent occurrences of events.  An
 * image \c A is a set of tri-tuple (x, y, count), representing the number of
 * occurreces (count) of event \c A at time slot \c x and component slot \c y.
 * <tt>A[x,y]</tt> is a short hand for the count of \c A at <tt>(x,y)</tt>.
 *
 * <code>Idx(A)</code> is a set of index of pixels of \c A, described as
 * \code
 *     Idx(A) := { (x, y) | all (x, y, z) in A }
 * \endcode
 *
 * \subsection imgintersect IMAGE INTERSECTION and CO-OCCURRENCES
 * Image intersection is defined as the following.
 * \code
 *    I(A, B) := { (x, y, min(A[x,y], B[x,y])) | all (x,y) in (Idx(A)^Idx(B)) }
 * \endcode
 * In other words, the intersection of \c A and \c B is the pixel-wise minimum
 * of the two image. The intersection is also used to represent the
 * co-occurrences of the two events.
 *
 * \subsection blackwhite BLACK-WHITE IMAGE
 * Black-white co-occurrences discard 'count' information in the image pixels.
 * The count information can be seen as intensity in gray-scale image. Hence,
 * the image with count being 0 or 1 (discarding the count) can be seen as
 * black-white image, and is defined as follow:
 * \code
 *    bw(A) := { (x, y, 1) | all (x, y, z) in A }
 * \endcode
 * Black-white images are useful in the situation of event count imbalance. For
 * example, we might have repetitive overheat value from metric data, but have
 * only single overheat message. With count, the high metric value will not be
 * associated with the overheat message. Discarding count will make them equals
 * (in terms of exist or not exist in the spatio-temporal space).
 *
 * \subsection offset PIXEL OFFSET
 * Target image (the right-hand-side of the association rule) can be shifted, so
 * that association to the future or past event can be done. If the offset
 * (option -o or --offset) is given as a positive number \c X, the target image
 * will be shifted to the right by \c X pixel in the rule evaluation. If \c X is
 * negative number, the target image will be shifted to the left by \c X pixel.
 *
 * REMARK: Negative \c X means associating current cause to FUTURE effect.
 *
 * \section example EXAMPLES
 *
 * To initialize workspace that works with 1-hour-1-node pixel images:
 * \par
 * \code
 *     bassoc -w workspace -c -S 3600 -N 1
 * \endcode
 *
 * To extract images from balerd's store with the data from '2015-01-01
 * 00:00:00' to current:
 * \par
 * \code
 *     bassoc -w workspace -x -s balerd_store -B '2015-01-01 00:00:00' -R recipe
 * \endcode
 *
 * Example content in the recipe file
 * \par
 * \code
 *     ev1: 128,129
 *     ev2: 150
 *     ev3: 151
 *     +MemFree: 1e+06,1e+07,1e+08,1e+09
 * \endcode
 *
 * In the above recipe example, the image 'ev1' is created from patterns 128 and
 * 129, while the rest are straighforwardly defined by a single pattern. The
 * 'MemFree' is a metric, so the recipe is actually a binning definition.
 *
 * To extract more images from balerd's store that are not specified in the
 * recipe file:
 * \par
 * \code
 *     bassoc -w workspace -x -s balerd_store -B '2015-01-01 00:00:00' \\
 *            -r 'ev4:200' -r 'ev5:600'
 * \endcode
 *
 * The above example will create ev4 and ev5 images from pattern IDs 200 and 600
 * respectively (under the same time constrain as recipe file example).
 *
 * To extract metric images, just pipe CSV metric data to bassoc, with the same
 * recipe file, as following:
 * \par
 * \code
 *     cat metric.csv | bassoc -w workspace -X -R recipe
 * \endcode
 *
 * \warning bassoc expects the first row to be the header row, containing column
 * names (metric names). The first column is expected to be unix timestamp, and
 * the second column is expected to be node id. The names of the first two
 * columns are not important.
 *
 * You can also extract the message occurrences and metric binning altogether
 * as follows:
 * \par
 * \code
 *     cat metric.csv | bassoc -w workspace -X -s balerd_store \\
 *                             -B '2015-01-01 00:00:00' -R recipe
 *     # Notice the captial 'X', not lower-case 'x'.
 * \endcode
 *
 * To mine rules with ev3 and ev5 being targets, with time-axis of the target
 * shifting to the right by -1 pixel (so that we can use the rule for future
 * prediction), significance theshold 0.01, confidence threshold 0.75:
 * \par
 * \code
 *     bassoc -w workspace -m ev3 -m ev5 -o -1 -S 0.01 -K 0.75
 * \endcode
 *
 * or:
 * \par
 * \code
 *     bassoc -w workspace -M target_file -o -1 -S 0.01 -K 0.75
 * \endcode
 * where \e target_file contains the following content:
 * \par
 * \code
 *     ev3
 *     # Comment is OK
 *     ev5
 * \endcode
 *
 * For black/white evaluation, just add '-b' flag.
 * \par
 * \code
 *     bassoc -w workspace -m ev3 -m ev5 -b -o -1 -S 0.01 -K 0.75
 *     # or
 *     bassoc -w workspace -M target_file -b -o -1 -S 0.01 -K 0.75
 * \endcode
 *
 * \note A rule (X->Y) of 0.1 significance means that XY co-occurrences
 * contribute 10% of the occurrences of Y. The significance threshold will
 * filter out the rules that has lesser significance.
 *
 * \note A rule (X->Y) of 0.9 confidence menas that 90% of the occurrences of X,
 * Y also occur. The confidence threshold will be used to accept rule candidates
 * that has greater confidence as rules.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <wordexp.h>

#include "bassoc.h"
#include "../baler/bhash.h"
#include "../baler/bheap.h"
#include "../baler/butils.h"
#include "../baler/barray.h"
#include "../baler/bmqueue.h"
#include "../baler/bset.h"
#include "../baler/bstore.h"
#include "../query/bquery.h"

/***** OPTIONS *****/
const char *short_opt = "hicw:t:n:xXs:B:E:H:r:R:o:m:M:z:K:S:D:v:bO:p:?";
struct option long_opt[] = {
	{"help",                    0,  0,  'h'},
	{"info",                    0,  0,  'i'},
	{"create",                  0,  0,  'c'},
	{"workspace",               1,  0,  'w'},
	{"sec-per-pixel",           1,  0,  't'},
	{"node-per-pixel",          1,  0,  'n'},
	{"extract",                 0,  0,  'x'},
	{"extract-w-metric",        0,  0,  'X'},
	{"store",                   1,  0,  's'},
	{"store-plugin",            1,  0,  'p'},
	{"ts-begin",                1,  0,  'B'},
	{"ts-end",                  1,  0,  'E'},
	{"host-ids",                1,  0,  'H'},
	{"recipe",                  1,  0,  'r'},
	{"recipe-file",             1,  0,  'R'},
	{"offset",                  1,  0,  'o'},
	{"mine-target",             1,  0,  'm'},
	{"mine-target-file",        1,  0,  'M'},
	{"threads",                 1,  0,  'z'},
	{"confidence-threshold",    1,  0,  'K'},
	{"significance-threshold",  1,  0,  'S'},
	{"difference-threshold",    1,  0,  'D'},
	{"black-white",             0,  0,  'b'},
	{"output",                  1,  0,  'O'},
	{"verbose",                 1,  0,  'v'},
	{0,                         0,  0,  0},
};

/***** GLOBAL VARIABLES *****/
enum {
	RUN_MODE_CREATE   =  0x1,
	RUN_MODE_EXTRACT  =  0x2,
	RUN_MODE_MINE     =  0x4,
	RUN_MODE_INFO     =  0x8,
} run_mode_flag = 0;

struct ptrlistentry {
	void *ptr;
	LIST_ENTRY(ptrlistentry) entry;
};

LIST_HEAD(ptrlist, ptrlistentry);

struct bassocimg_cache *img_cache;
struct bassocimg_cache *target_cache;

const char *output_path = NULL;
FILE *rule_out;

const char *workspace_path = NULL;
uint32_t spp = 3600;
uint32_t npp = 1;
const char *store_path = NULL;
const char *store_plugin = "bstore_htbl";
const char *ts_begin = NULL;
const char *ts_end = NULL;
const char *host_ids = NULL;
const char *recipe_file_path = NULL;
double confidence = 0.75;
double significance = 0.10;
double difference = 0.15;
int offset = 0;
const char *mine_target_file_path = NULL;
time_t time_begin = 0;
time_t time_end = 0;

int blackwhite = 0;

int enable_metric_stream = 0;

struct bassoc_conf_handle *conf_handle;

struct bhash *ptn2imglist;
struct bhash *metric2imgbin;

struct {
	struct bdstr *conf;
	struct bdstr *img_dir;
	struct bdstr *img_cache_path;
	struct bdstr *target_cache_path;
} paths;

struct barray *cli_recipe = NULL;

struct barray *cli_targets = NULL;

struct barray *images = NULL;

struct barray *target_images = NULL;
struct bhash *target_images_hash = NULL;

struct bassoc_rule_subq {
	bmqueue_t bmq;
	uint64_t refcount;
};

struct bassoc_rule_q {
	struct bassoc_rule_subq subq[2];
	struct bassoc_rule_subq *current_subq;
	struct bassoc_rule_subq *next_subq;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	enum {
		BASSOC_RULE_Q_STATE_ACTIVE = 0,
		BASSOC_RULE_Q_STATE_DONE,
		BASSOC_RULE_Q_STATE_LVL_DONE, /* done within level */
	} state;
};

struct bassoc_rule_q rule_q;
pthread_t *miner = NULL;
int miner_threads = 1;
struct bassoc_rule_index *rule_index = NULL;
struct bset_u64 *comp_set = NULL;
uint32_t bin_width = 0;

/***** FUNCTIONS *****/

void usage()
{
	printf(
"SYNOPSIS: \n"
"	creating a workspace: \n"
"		bassoc -w WORKSPACE -c [-t NUMBER] [-n NUMBER]\n"
"\n"
"	extracting images: \n"
"		bassoc -w WORKSPACE -x -p BSTORE_PLUGIN -s BALERD_STORE\n"
"					[-B TS] [-E TS]\n"
"					[-H IDS] -R RECIPE_FILE\n"
"\n"
"		bassoc -w WORKSPACE -x -p BSTORE_PLUGIN -s BALERD_STORE\n"
"					[-B TS] [-E TS]\n"
"					[-H IDS] -r RECIPE1 -r RECIPE2 ...\n"
"\n"
"	mine for associations: \n"
"		bassoc -w WORKSPACE [-o NUMBER] -m TARGET_LIST\n"
"\n"
"		bassoc -w WORKSPACE [-o NUMBER] -M TARGET_FILE\n"
"\n"
"See bassoc(1) for more information."
	);
}

static
uint32_t __get_bin_width()
{
	uint32_t bin[] = {DAY, HOUR, MINUTE};
	uint32_t i;
	for (i = 0; i < sizeof(bin)/sizeof(bin[0]); i++) {
		if (conf_handle->conf->spp >= bin[i])
			return bin[i];
	}
	return 0;
}

bcomp_hist_iter_hent_t bcomp_hist_iter_hent_new(bstore_t bs, uint64_t comp_id,
						time_t ts_begin,
						time_t ts_end,
						uint64_t bin_width)
{
	bcomp_hist_iter_hent_t hiter = calloc(1, sizeof(*hiter));
	if (!hiter)
		return NULL;
	hiter->iter = bstore_comp_hist_iter_new(bs);
	if (!hiter->iter) {
		goto err0;
	}
	hiter->comp_id = comp_id;
	hiter->ts_begin = ts_begin;
	hiter->ts_end = ts_end;
	hiter->bin_width = bin_width;
	return hiter;
err0:
	free(hiter);
	return NULL;
}

void bcomp_hist_iter_hent_free(bcomp_hist_iter_hent_t hiter)
{
	return; /* do nothing!!! */
	bstore_comp_hist_iter_free(hiter->iter);
	free(hiter);
}

int bcomp_hist_iter_hent_first(bcomp_hist_iter_hent_t hiter)
{
	bcomp_hist_t h;
	hiter->valid = 0;
	hiter->hist.comp_id = hiter->comp_id;
	hiter->hist.time = hiter->ts_begin;
	hiter->hist.bin_width = hiter->bin_width;
	h = bstore_comp_hist_iter_find(hiter->iter, &hiter->hist);
	if (!h)
		return ENOENT;
	hiter->valid = 1;
	return 0;
}

int bcomp_hist_iter_hent_next(bcomp_hist_iter_hent_t hiter)
{
	bcomp_hist_t h;
	time_t last_time;
	if (!hiter->valid)
		return EINVAL;
	last_time = hiter->hist.time;
	hiter->valid = 0;
	h = bstore_comp_hist_iter_next(hiter->iter, &hiter->hist);
	if (!h)
		return ENOENT;
	if (hiter->ts_end && hiter->ts_end < hiter->hist.time)
		return ENOENT;
	hiter->valid = 1;
	assert(hiter->comp_id == hiter->hist.comp_id);
	assert(last_time <= hiter->hist.time);
	return 0;
}

int bcomp_hist_iter_hent_cmp(void *_a, void *_b)
{
	bcomp_hist_iter_hent_t a = _a;
	bcomp_hist_iter_hent_t b = _b;
	/* using min heap ==> invalid iterator is the greatest */
	if (!a->valid)
		return 1;
	if (!b->valid)
		return -1;
	/* both valid, compare current hist entry */
	if (a->hist.time < b->hist.time)
		return -1;
	if (a->hist.time > b->hist.time)
		return 1;
	if (a->hist.comp_id < b->hist.comp_id)
		return -1;
	if (a->hist.comp_id > b->hist.comp_id)
		return 1;
	return 0;
}

struct bassocimgbin *bassocimgbin_new(int alloc_bin_len)
{
	struct bassocimgbin *bin = calloc(1, sizeof(*bin) +
				alloc_bin_len * sizeof(*bin->bin));
	if (!bin)
		return NULL;
	bin->alloc_bin_len = alloc_bin_len;
	bin->bin_len = 2;
	bin->bin[0].lower_bound = -INFINITY;
	bin->bin[1].lower_bound = INFINITY;
	return bin;
}

void bassocimgbin_free(struct bassocimgbin *bin)
{
	int i;
	for (i = 0; i < bin->bin_len; i++) {
		if (bin->bin[i].img)
			bassocimg_put(bin->bin[i].img);
	}
	free(bin);
}

int bassocimgbin_addbin(struct bassocimgbin *bin, double value)
{
	if (bin->bin_len == bin->alloc_bin_len) {
		return ENOMEM;
	}
	if (bin->bin[bin->bin_len - 2].lower_bound > value) {
		return EINVAL;
	}
	bin->bin[bin->bin_len - 1].lower_bound = value;
	bin->bin[bin->bin_len].lower_bound = INFINITY;
	bin->bin_len++;
	return 0;
}

struct bassocimgbin *bassocimgbin_expand(struct bassocimgbin *bin, int inc_bin_len)
{
	int new_alloc_len = bin->alloc_bin_len + inc_bin_len;
	bin = realloc(bin, sizeof(*bin) + new_alloc_len * sizeof(*bin->bin));
	if (!bin)
		return NULL;
	bin->alloc_bin_len = new_alloc_len;
	return bin;
}

int bassocimgbin_getimgname(struct bassocimgbin *bin, int binidx,
							struct bdbstr *bdbstr)
{
	if (bin->bin_len - 2 < binidx)
		return EINVAL;
	return bdbstr_append_printf(bdbstr, "%s[%lf,%lf)", bin->metric_name,
						bin->bin[binidx].lower_bound,
						bin->bin[binidx+1].lower_bound);
}

int bassocimgbin_getbinidx(struct bassocimgbin *bin, double value)
{
	int l = 0, r = bin->bin_len - 1;
	int c;
	while (l < r) {
		c = (l+r)/2;
		if (value < bin->bin[c].lower_bound) {
			/* go to the left */
			r = c - 1;
			continue;
		}
		if (bin->bin[c+1].lower_bound <= value) {
			/* go to the right */
			l = c + 1;
			continue;
		}
		return c;
	}
	if (l == r)
		return l;
	return -1;
}

int bassocimgbin_flush(struct bassocimgbin *bin, uint32_t ts)
{
	struct bassocimg_pixel pxl;
	struct bassocimg *img;
	struct barray *count_buff;
	int rc;
	int i, j, n;
	uint32_t *count;
	pxl.sec = ts;
	for (i = 0; i < bin->bin_len - 1; i++) {
		img = bin->bin[i].img;
		count_buff = bin->bin[i].count_buff;
		if (!count_buff)
			continue;
		n = barray_get_len(count_buff);
		for (j = 0; j < n; j++) {
			count = (void*)barray_get(count_buff, j, NULL);
			if (count && *count) {
				pxl.comp_id = j;
				pxl.count = *count;
				rc = bassocimg_add_count(img, &pxl);
				if (rc)
					return rc;
				*count = 0;
			}
		}
	}

	return 0;
}

static
int img_addptn(struct bassocimg *img, int ptn_id)
{
	int rc = 0;
	struct ptrlist *list;
	struct ptrlistentry *lent;
	struct bhash_entry *ent = bhash_entry_get(ptn2imglist,
					(void*)&ptn_id, sizeof(ptn_id));
	if (!ent) {
		list = malloc(sizeof(*list));
		if (!list) {
			rc = ENOMEM;
			goto out;
		}
		LIST_INIT(list);
		ent = bhash_entry_set(ptn2imglist, (void*)&ptn_id,
					sizeof(ptn_id), (uint64_t)(void*)list);
	}
	list = (void*)ent->value;
	lent = malloc(sizeof(*lent));
	if (!lent){
		rc = ENOMEM;
		goto out;
	}
	lent->ptr = img;
	LIST_INSERT_HEAD(list, lent, entry);
out:
	return rc;
}

int bassocimgbin_open(struct bassocimgbin *bin, int idx, struct bdbstr *name)
{
	struct bassocimg *img = NULL;
	int rc;
	bdbstr_reset(name);
	rc = bassocimgbin_getimgname(bin, idx, name);
	if (rc)
		return rc;
	img = bassocimg_cache_get_img(img_cache, name->bstr, 1);
	bin->bin[idx].img = img;
	bin->bin[idx].count_buff = barray_alloc(sizeof(uint32_t), 1024);
	if (!bin->bin[idx].count_buff)
		return ENOMEM;
cleanup:
	return 0;
}

static
int handle_ptn_recipe(const char *recipe)
{
	int rc = 0;
	int len;
	struct bdbstr *bdbstr = bdbstr_new(512);
	char *str;
	int a, b, n, i;
	struct bassocimg *img;
	if (!bdbstr) {
		rc = ENOMEM;
		goto out;
	}
	str = strchr(recipe, ':');
	if (!str) {
		rc = EINVAL;
		berr("bad recipe: %s", recipe);
		goto out;
	}

	rc = bdbstr_append_printf(bdbstr, "%.*s", (int)(str-recipe), recipe);
	if (rc) {
		goto out;
	}

	img = bassocimg_cache_get_img(img_cache, bdbstr->bstr, 1);
	if (!img) {
		berr("Cannot create image: %.*s", bdbstr->bstr->blen, bdbstr->bstr->cstr);
		rc = errno;
		goto out;
	}

	/* str points at ':', move it */
	str++;
	while (*str) {
		n = sscanf(str, "%d%n - %d%n", &a, &len, &b, &len);
		switch (n) {
		case 1:
			b = a;
			break;
		case 2:
			/* do nothing */
			break;
		default:
			berr("Bad recipe: %s", recipe);
			rc = EINVAL;
			goto out;
		}
		str += len;
		for (i = a; i <= b; i++) {
			/* add img ref for ptn i */
			rc = img_addptn(img, i);
			if (rc)
				goto out;
		}
		while (isspace(*str) || *str == ',')
			str++;
	}

out:
	if (bdbstr)
		free(bdbstr);
	return 0;
}

static
int handle_metric_recipe(const char *recipe)
{
	struct bhash_entry *hent;
	struct bassocimgbin *bin = NULL;
	const char *s, *t;
	double x;
	int n, rc;

	/* skip white spaces */
	while (*recipe && isspace(*recipe)) recipe++;

	s = strchr(recipe, ':');
	if (!s) {
		rc = EINVAL;
		goto err;
	}

	hent = bhash_entry_get(metric2imgbin, recipe, s - recipe);
	if (hent) {
		berr("Multiple definition of %.*s", (int)(s - recipe), recipe);
		rc = EEXIST;
		goto err;
	}

	n = 0;
	t = s + 1; /* s pointed at ':' */
	while (*t) {
		if (*t == ',')
			n++;
		t++;
	}
	n += 3;

	bin = bassocimgbin_new(n);
	n = snprintf(bin->metric_name, sizeof(bin->metric_name), "%.*s",
						(int)(s - recipe), recipe);
	if (n >= sizeof(bin->metric_name)) {
		rc = EINVAL;
		goto err;
	}

	t = s + 1; /* s pointed at ':' */
	while (*t) {
		n = 0;
		sscanf(t, "%lf%n", &x, &n);
		if (!n) {
			rc = EINVAL;
			goto err;
		}
		rc = bassocimgbin_addbin(bin, x);
		if (rc) {
			return rc;
		}
		/* next token */
		t += n;
		while (isspace(*t) || *t == ',')
			t++;
	}

	hent = bhash_entry_set(metric2imgbin, recipe, s - recipe,
							(uint64_t)bin);
	if (!hent) {
		rc = ENOMEM;
		goto err;
	}

	return 0;

err:
	if (bin)
		bassocimgbin_free(bin);
	return rc;
}

static
int handle_recipe(const char *recipe)
{
	while (*recipe && isspace(*recipe)) {
		recipe++;
	}
	if (*recipe == '+') {
		return handle_metric_recipe(recipe+1);
	}
	return handle_ptn_recipe(recipe);
}

void handle_args(int argc, char **argv)
{
	char c;
	int rc;
loop:
	c = getopt_long(argc, argv, short_opt, long_opt, NULL);
	switch (c) {
	case -1:
		goto end;
		break;
	case 'i':
		run_mode_flag |= RUN_MODE_INFO;
		break;
	case 'c':
		run_mode_flag |= RUN_MODE_CREATE;
		break;
	case 'w':
		workspace_path = optarg;
		break;
	case 't':
		spp = atoi(optarg);
		break;
	case 'n':
		npp = atoi(optarg);
		break;
	case 'x':
		run_mode_flag |= RUN_MODE_EXTRACT;
		break;
	case 'X':
		run_mode_flag |= RUN_MODE_EXTRACT;
		enable_metric_stream = 1;
		break;
	case 's':
		store_path = optarg;
		break;
	case 'p':
		store_plugin = optarg;
		break;
	case 'B':
		ts_begin = optarg;
		time_begin = bparse_ts(ts_begin);
		if (time_begin == -1) {
			berr("Unknown time format: %s", ts_begin);
			exit(-1);
		}
		break;
	case 'E':
		ts_end = optarg;
		time_end = bparse_ts(ts_end);
		if (time_end == -1) {
			berr("Unknown time format: %s", ts_begin);
			exit(-1);
		}
		break;
	case 'H':
		host_ids = optarg;
		break;
	case 'r':
		rc = barray_append(cli_recipe, &optarg);
		if (rc) {
			berr("barray_append() error, rc: %d", rc);
			exit(-1);
		}
		break;
	case 'R':
		recipe_file_path = optarg;
		break;
	case 'o':
		offset = atoi(optarg);
		break;
	case 'm':
		run_mode_flag |= RUN_MODE_MINE;
		rc = barray_append(cli_targets, &optarg);
		if (rc) {
			berr("barray_append() error, rc: %d", rc);
			exit(-1);
		}
		break;
	case 'M':
		run_mode_flag |= RUN_MODE_MINE;
		mine_target_file_path = optarg;
		break;
	case 'z':
		miner_threads = atoi(optarg);
		break;
	case 'K':
		confidence = atof(optarg);
		break;
	case 'S':
		significance = atof(optarg);
		break;
	case 'D':
		difference = atof(optarg);
		break;
	case 'b':
		blackwhite = 1;
		break;
	case 'O':
		output_path = optarg;
		break;
	case 'v':
		rc = blog_set_level_str(optarg);
		if (rc) {
			berr("Invalid verbosity level: %s", optarg);
			exit(-1);
		}
		break;
	case 'h':
	default:
		usage();
		exit(-1);
	}
	goto loop;

end:
	if (workspace_path == NULL) {
		berr("workspace path (-w) is needed");
		exit(-1);
	}

	if (output_path) {
		rule_out = fopen(output_path, "w");
		if (!rule_out) {
			berr("Cannot open rule output file: %s", output_path);
			exit(-1);
		}
	} else {
		rule_out = stdout;
	}

	rc = setvbuf(rule_out, NULL, _IONBF, 0);
	assert(rc == 0);

	size_t len = strlen(workspace_path);
	paths.conf = bdstr_new(512);
	paths.img_dir = bdstr_new(512);
	paths.img_cache_path = bdstr_new(512);
	paths.target_cache_path = bdstr_new(512);

	if (!paths.conf
			|| !paths.img_dir
			|| !paths.img_cache_path
			|| !paths.target_cache_path) {
		berr("Out of memory");
		exit(-1);
	}

	bdstr_append_printf(paths.conf, "%s/conf", workspace_path);
	bdstr_append_printf(paths.img_dir, "%s/img", workspace_path);
	bdstr_append_printf(paths.img_cache_path, "%s/img_cache", workspace_path);
	bdstr_append_printf(paths.target_cache_path, "%s/target_cache", workspace_path);

	return;
}

static
void open_cache_routine()
{
	img_cache = bassocimg_cache_open(paths.img_cache_path->str, 0);
	if (!img_cache) {
		berr("bassocimg_cache_open() failed, errno: %d, path: %s\n",
				paths.img_cache_path->str, errno);
		exit(-1);
	}
	target_cache = bassocimg_cache_open(paths.target_cache_path->str, 0);
	if (!target_cache) {
		berr("bassocimg_cache_open() failed, errno: %d, path: %s\n",
				paths.img_cache_path->str, errno);
		exit(-1);
	}
}

static
void __create_dir(const char *dir, mode_t mode)
{
	int rc = bmkdir_p(dir, mode);
	if (rc) {
		berr("Cannot create dir '%s', rc: %d", dir, rc);
		exit(-1);
	}
}

void bassoc_conf_close(struct bassoc_conf_handle *handle)
{
	int rc;
	if (handle->conf) {
		rc = munmap(handle->conf, sizeof(*handle->conf));
		if (rc)
			bwarn("munmap() rc: %d, errno(%d): %m", rc, errno);
	}
	if (handle->fd) {
		rc = close(handle->fd);
		if (rc)
			bwarn("close() rc: %d, errno(%d): %m", rc, errno);
	}
	free(handle);
}

struct bassoc_conf_handle *bassoc_conf_open(const char *path, int flags, ...)
{
	off_t off;
	ssize_t sz;
	struct bassoc_conf_handle *handle = malloc(sizeof(*handle));
	if (!handle)
		return NULL;
	handle->fd = -1;
	handle->conf = 0;
	va_list ap;
	mode_t mode;

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
		handle->fd = open(path, flags, mode);
		if (handle->fd < 0) {
			berr("Cannot open file %s, err(%d): %m", path, errno);
			goto err;
		}
		off = lseek(handle->fd, sizeof(*handle->conf) - 1, SEEK_SET);
		if (off == -1) {
			berr("seek(%s) error(%d): %m", path, errno);
			goto err;
		}
		sz = write(handle->fd, "", 1);
		if (sz == -1) {
			berr("write(%s) error(%d): %m", path, errno);
			goto err;
		}
	} else {
		handle->fd = open(path, flags);
	}
	handle->conf = mmap(NULL, sizeof(*handle->conf), PROT_READ|PROT_WRITE,
			MAP_SHARED, handle->fd, 0);
	if (handle->conf == MAP_FAILED) {
		berr("mmap(%s) error(%d): %m", path, errno);
		goto err;
	}
	return handle;
err:
	bassoc_conf_close(handle);
	return NULL;
}

static
void open_bassoc_conf_routine()
{
	conf_handle = bassoc_conf_open(paths.conf->str, O_RDWR);
	if (!conf_handle) {
		berr("Cannot open bassoc configuration");
		exit(-1);
	}
}

void create_routine()
{
	int rc;
	static char buff[4096];
	if (bfile_exists(workspace_path)) {
		berr("workspace '%s' existed", workspace_path);
		exit(-1);
	}

	__create_dir(workspace_path, 0755);
	__create_dir(paths.img_dir->str, 0755);

	conf_handle = bassoc_conf_open(paths.conf->str, O_RDWR|O_CREAT, 0666);
	if (!conf_handle) {
		berr("Cannot open-create bassoc configuration");
		exit(-1);
	}
	conf_handle->conf->npp = npp;
	conf_handle->conf->spp = spp;
	bassoc_conf_close(conf_handle);

	/* create img_cache */
	img_cache = bassocimg_cache_open(paths.img_cache_path->str, 1);
	if (!img_cache) {
		berr("Cannot create image cache: %s", paths.img_cache_path->str);
		exit(-1);
	}
	bassocimg_cache_close_free(img_cache);

	/* create target_cache */
	target_cache = bassocimg_cache_open(paths.target_cache_path->str, 1);
	if (!target_cache) {
		berr("Cannot create image cache: %s", paths.target_cache_path->str);
		exit(-1);
	}
	bassocimg_cache_close_free(target_cache);
}

int process_recipe_line(char *line, void *ctxt)
{
	int rc;
	rc = handle_recipe(line);
	if (rc) {
		/* Detail of the error should be printed in
		 * handle_recipe() function */
		berr("Recipe error ... exiting");
		exit(-1);
	}
	return 0;
}

static
void process_recpies_routine()
{
	int i, n, rc;
	const char *recipe;
	FILE *fin;

	/* Iterate through recipes from CLI */
	n = (cli_recipe)?(barray_get_len(cli_recipe)):(0);
	for (i = 0; i < n; i++) {
		barray_get(cli_recipe, i, &recipe);
		rc = handle_recipe(recipe);
		if (rc) {
			/* Detail of the error should be printed in
			 * handle_recipe() function */
			berr("Recipe error ... exiting");
			exit(-1);
		}
	}

	if (!recipe_file_path) {
		return;
		/* no need to continue */
	}

	/* Iterate through recipes from RECIPE_FILE */
	rc = bprocess_file_by_line_w_comment(recipe_file_path,
						process_recipe_line, NULL);
	if (rc) {
		berr("Process recipe file '%s' error, rc: %d",
							recipe_file_path, rc);
		exit(-1);
	}
}

struct __best_img {
	int spp;
	int npp;
};

static
void __bq_imgiter_cb(const char *name, void *ctxt)
{
	struct __best_img *best = ctxt;
	int _npp, _spp, n;
	n = sscanf(name, "%d-%d", &_spp, &_npp);
	if (n != 2) {
		bwarn("Unexpected image name: %s", name);
		return;
	}
	if (_npp > conf_handle->conf->npp)
		/* not fine enough */
		return;
	if (_spp > conf_handle->conf->spp)
		/* not fine enough */
		return;
	if (best->spp && (best->npp*best->spp >= _npp*spp))
		/* finer than needed */
		return;
	/* better image, update the 'best' */
	best->npp = _npp;
	best->spp = _spp;
}

int get_closest_img_store(struct bq_store *bq_store, struct bdstr *bdstr)
{
	struct __best_img best = {0, 0};
	int rc = bq_imgstore_iterate(bq_store, __bq_imgiter_cb, &best);
	if (rc)
		return rc;
	if (!best.spp)
		return ENOENT;
	bdstr_reset(bdstr);
	bdstr_append_printf(bdstr, "%d-%d", best.spp, best.npp);
	return 0;
}

static inline
int __pxl_cmp(uint32_t ts0, uint32_t c0, uint32_t ts1, uint32_t c1)
{
	if (ts0 < ts1)
		return -1;
	if (ts0 > ts1)
		return 1;
	if (c0 < c1)
		return -1;
	if (c0 > c1)
		return 1;
	return 0;
}

static
void extract_routine_by_msg(struct bq_store *bq_store)
{
	berr("Extracting images by messages is not yet implemented.");
	exit(-1);
}

static
int __bq_cmp(struct bimgquery *bq0, struct bimgquery *bq1)
{
	struct bpixel p0;
	struct bpixel p1;
	bq_img_entry_get_pixel(bq0, &p0);
	bq_img_entry_get_pixel(bq1, &p1);
	if (p0.sec < p1.sec)
		return -1;
	if (p0.sec > p1.sec)
		return 1;
	if (p0.comp_id < p1.comp_id)
		return -1;
	if (p0.comp_id > p1.comp_id)
		return 1;
	return 0;
}

static
struct bheap *__heap_init(struct bq_store *bq_store, const char *img_store_name)
{
	char buff[128];
	struct bhash_iter *hiter = bhash_iter_new(ptn2imglist);
	if (!hiter) {
		berror("bhash_iter_new()");
		exit(-1);
	}
	struct bheap *h = bheap_new(65536, (void*)__bq_cmp);
	if (!h) {
		berror("bheap_new()");
		exit(-1);
	}

	struct bimgquery *bq;
	struct bhash_entry *hent;
	uint32_t ptn_id;
	int rc;

	rc = bhash_iter_begin(hiter);
	if (rc) {
		berror("bhash_iter_begin()");
		exit(-1);
	}
	while (rc == 0) {
		hent = bhash_iter_entry(hiter);
		ptn_id = *(uint32_t*)hent->key;
		snprintf(buff, sizeof(buff), "%u", ptn_id);
		bq = bimgquery_create(bq_store, host_ids, buff,
					ts_begin, ts_end, img_store_name, &rc);
		if (!bq) {
			berror("bimgquery_create()");
			exit(-1);
		}
		rc = bq_first_entry((void*)bq);
		if (rc == ENOENT) {
			bimgquery_destroy(bq);
			goto skip;
		}

		if (rc) {
			berror("bq_first_entry()");
			exit(-1);
		}

		rc = bheap_insert(h, bq);
		if (rc) {
			berror("bheap_insert()");
			exit(-1);
		}
	skip:
		rc = bhash_iter_next(hiter);
	}

	return h;
}

static
struct bheap *__heap4_init(bstore_t bs, struct bset_u64 *comp_set, uint32_t bin_width)
{
	char buff[128];
	struct bset_u64_iter *set_iter = NULL;
	struct bheap *h = NULL;
	uint64_t comp_id;
	int rc, i;

	set_iter = bset_u64_iter_new(comp_set);
	if (!set_iter) {
		berror("bset_u64_iter_new()");
		goto out;
	}

	h = bheap_new(comp_set->count, bcomp_hist_iter_hent_cmp);
	if (!h) {
		berror("bheap_new()");
		goto cleanup;
	}

	while (0 == (rc = bset_u64_iter_next(set_iter, &comp_id))) {
		bcomp_hist_iter_hent_t hiter = bcomp_hist_iter_hent_new(bs,
				comp_id, time_begin, time_end, bin_width);
		if (!hiter)
			goto err0;
		rc = bcomp_hist_iter_hent_first(hiter);
		if (rc) {
			bcomp_hist_iter_hent_free(hiter);
			continue;
		}
		rc = bheap_insert(h, hiter);
		if (rc) {
			bcomp_hist_iter_hent_free(hiter);
			goto err0;
		}
	}

	goto cleanup;

err0:
	for (i = 0; i < h->len; i++) {
		bcomp_hist_iter_hent_free(h->array[i]);
	}
	bheap_free(h);
	h = NULL;

cleanup:
	if (set_iter)
		bset_u64_iter_free(set_iter);
out:
	return h;
}

static
int __heap_get_pixel(struct bheap *bheap, struct bpixel *pixel)
{
	struct bimgquery *bq = bheap_get_top(bheap);
	if (!bq)
		return ENOENT;
	return bq_img_entry_get_pixel(bq, pixel);
}

static
int __heap4_get_pixel(struct bheap *h, struct bpixel *pixel)
{
	bcomp_hist_iter_hent_t hent = bheap_get_top(h);
	if (!hent || !hent->valid)
		return ENOENT;
	pixel->count = hent->hist.msg_count;
	pixel->ptn_id = hent->hist.ptn_id;
	pixel->sec = hent->hist.time;
	pixel->comp_id = hent->hist.comp_id;
	return 0;
}

static
int __heap_next_entry(struct bheap *bheap)
{
	int rc;
	struct bimgquery *bq = bheap_get_top(bheap);
	if (!bq)
		return ENOENT;

	rc = bq_next_entry((void*)bq);
	switch (rc) {
	case 0:
		/* bq is still good, just percolate it */
		bheap_percolate_top(bheap);
		return 0;
	case ENOENT:
		/* end of this bq --> destroy */
		bheap_remove_top(bheap);
		bimgquery_destroy(bq);
		if (bheap_get_top(bheap) == NULL)
			/* no more bq in the heap */
			return ENOENT;
		return 0;
	/* For other rc, just return it as-is */
	}

	return rc;
}

static
int __heap4_next_entry(struct bheap *bheap)
{
	int rc;
	bcomp_hist_iter_hent_t hiter = bheap_get_top(bheap);
	if (!hiter)
		return ENOENT;
	assert(hiter->valid);

	rc = bcomp_hist_iter_hent_next(hiter);

	switch (rc) {
	case 0:
		/* hent is still good, just percolate it */
		bheap_percolate_top(bheap);
		return 0;
	case ENOENT:
		/* end of this bq --> destroy */
		bheap_remove_top(bheap);
		bcomp_hist_iter_hent_free(hiter);
		if (bheap_get_top(bheap) == NULL)
			/* no more bq in the heap */
			return ENOENT;
		return 0;
	/* For other rc, just return it as-is */
	}

	return rc;
}

static
void extract_routine_by_img(struct bq_store *bq_store, const char *img_store_name)
{
	int i, n, rc;
	struct bimgquery *bq;
	struct bpixel pixel;
	struct bassocimg_pixel bassoc_pixel;
	struct bhash_entry *hent;
	struct ptrlist *list;
	struct ptrlistentry *lent;
	struct bheap *bheap;
	uint32_t npp = conf_handle->conf->npp;
	uint32_t spp = conf_handle->conf->spp;

	bheap = __heap_init(bq_store, img_store_name);
	assert(bheap);
	/* must check for the first entry */
	rc = __heap_get_pixel(bheap, &pixel);
	if (rc == ENOENT) {
		bwarn("No image entry ... please check recipes or extracting conditions.");
		return;
	}

loop:
	/* reaching here, pixel is guaranteed to be valid */
	__heap_get_pixel(bheap, &pixel);
	hent = bhash_entry_get(ptn2imglist, (void*)&pixel.ptn_id, sizeof(pixel.ptn_id));
	if (!hent)
		goto next;
	list = (void*)hent->value;
	LIST_FOREACH(lent, list, entry) {
		bassoc_pixel.sec = (pixel.sec / spp) * spp;
		bassoc_pixel.comp_id = (pixel.comp_id / npp) * npp;
		bassoc_pixel.count = pixel.count;
		rc = bassocimg_add_count(lent->ptr, &bassoc_pixel);
		if (rc) {
			berr("bassocimg_add_count() error, rc: %d", rc);
			exit(-1);
		}
	}

next:
	/* next entry */
	rc = __heap_next_entry(bheap);
	switch (rc) {
	case 0:
		goto loop;
	case ENOENT:
		break;
	default:
		berr("__heap_next_entry() error, rc: %d", rc);
	}
}

static
void extract_bq_store_routine()
{
	int i, n, rc;
	struct bq_store *bq_store;
	struct bimgquery *bq;
	struct bdstr *bdstr;
	struct bpixel pixel;

	bq_store = bq_open_store(store_path);
	if (!bq_store) {
		berr("Cannot open baler store (%s), err(%d): %m", store_path, errno);
		exit(-1);
	}
	bdstr = bdstr_new(128);
	if (!bdstr) {
		berr("Out of memory (in %s() %s:%d)", __func__, __FILE__, __LINE__);
		exit(-1);
	}

	rc = get_closest_img_store(bq_store, bdstr);
	switch (rc) {
	case 0:
		extract_routine_by_img(bq_store, bdstr->str);
		break;
	case ENOENT:
		extract_routine_by_msg(bq_store);
		break;
	default:
		berr("get_closest_img_store() error, rc: %d", rc);
		exit(-1);
	}
}

static
void extract_routine_by_hist(bstore_t bs)
{
	int i, n, rc;
	struct bimgquery *bq;
	struct bpixel pixel;
	struct bassocimg_pixel bassoc_pixel;
	struct bhash_entry *hent;
	struct ptrlist *list;
	struct ptrlistentry *lent;
	uint32_t npp = conf_handle->conf->npp;
	uint32_t spp = conf_handle->conf->spp;
	bcomp_hist_iter_t hiter;
	struct bcomp_hist_s comp_hist = {0};
	bcomp_hist_t hist;

	hiter = bstore_comp_hist_iter_new(bs);
	if (!hiter) {
		perror("bcomp_hist_iter_hent_new()");
		exit(-1);
	}

	comp_hist.bin_width = __get_bin_width();
	if (time_begin)
		comp_hist.time = time_begin;
	hist = bstore_comp_hist_iter_find(hiter, &comp_hist);
	if (!hist) {
		bwarn("No data matching given criteria ...");
		/* no need to continue ... */
		return;
	}

loop:
	/* hist ==> pixel */
	pixel.comp_id = hist->comp_id;
	pixel.ptn_id = hist->ptn_id;
	pixel.sec = hist->time;
	pixel.count = hist->msg_count;

	/* get list of images associated with the pattern ID */
	hent = bhash_entry_get(ptn2imglist, (void*)&pixel.ptn_id,
							sizeof(pixel.ptn_id));
	if (!hent)
		goto next;
	list = (void*)hent->value;
	LIST_FOREACH(lent, list, entry) {
		bassoc_pixel.sec = (pixel.sec / spp) * spp;
		bassoc_pixel.comp_id = (pixel.comp_id / npp) * npp;
		bassoc_pixel.count = pixel.count;
		rc = bassocimg_add_count(lent->ptr, &bassoc_pixel);
		if (rc) {
			berr("bassocimg_add_count() error, rc: %d", rc);
			exit(-1);
		}
	}

next:
	/* next entry */
	hist = bstore_comp_hist_iter_next(hiter, &comp_hist);
	if (!hist)
		goto out;
	if (time_end && hist->time > time_end)
		goto out;
	goto loop;
out:
	return;
}

static
void extract_routine_by_msg4(bstore_t bs)
{
	berr("NOT IMPLEMENTED HAHAHAHAHA!");
	exit(-1);
}

static
struct bset_u64 * __get_comp_set(bstore_t bs)
{
	struct bset_u64 *set = NULL;
	btkn_iter_t ti = NULL;
	btkn_t tkn;

	if (host_ids) {
		set = bset_u64_from_numlist(host_ids, 65537);
		goto out;
	}

	/* else, get all host IDs from the store */

	set = bset_u64_alloc(65537);
	if (!set)
		goto out;

	ti = bstore_tkn_iter_new(bs);
	for (tkn = bstore_tkn_iter_first(ti);
			tkn; tkn = bstore_tkn_iter_next(ti)) {
		if (!btkn_has_type(tkn, BTKN_TYPE_HOSTNAME))
			continue;
		bset_u64_insert(set, tkn->tkn_id);
	}
	// bstore_tkn_iter_free(ti);
out:
	return set;
}

static
void extract_bstore_routine()
{
	bstore_t bs = bstore_open(store_plugin, store_path, O_RDONLY);
	if (!bs) {
		berr("Cannot open store(%s): %s", store_plugin, store_path);
		exit(-1);
	}
	bin_width = __get_bin_width();
	comp_set = __get_comp_set(bs);
	if (!comp_set) {
		berr("Cannot create component set, errno: %d", errno);
		exit(-1);
	}
	if (!comp_set->count) {
		berr("Component set empty!");
		exit(-1);
	}
	if (bin_width > 0) {
		extract_routine_by_hist(bs);
	} else {
		extract_routine_by_msg4(bs);
	}
	bset_u64_free(comp_set);
	bstore_close(bs);
}

static
const char *__get_metric_input(int *row, int *col)
{
	static char buff[BUFSIZ+1];
	static char *str = buff;
	static char *end = buff;
	static int r = 0;
	static int c = 0;
	const char *ret;
	static ssize_t rsz = 0;
	static ssize_t sz = 0;
	int need_replenish = 0;
	int eol = 0;
	int eof = 0;
	int nl = 0;
	int rc;

	if (sz < 258)
		need_replenish = 1;

replenish:
	if (need_replenish && !eof) {
		need_replenish = 0;
		/* move the left-over to the beginning */
		memmove(buff, str, sz);
		buff[sz] = 0;

		/* replenishing buffer */
		rsz = read(0, buff + sz, BUFSIZ - sz);
		if (rsz < 0) {
			berr("read() error(%d): %m", errno);
			return NULL;
		}
		if (rsz == 0){
			eof = 1;
		}
		sz += rsz;
		buff[sz] = 0;
		str = buff;
	}

	if (!sz) {
		errno = ENOENT;
		return NULL;
	}

	rc = bcsv_get_cell(str, (void*)&end);
	rsz = end - str;
	if (rsz > 255) {
		/* name too big */
		berr("Input cell too big");
		errno = EMSGSIZE;
		return NULL;
	}

	if (rc) {
		need_replenish = 1;
		goto replenish;
	}

	switch (*end) {
	case '\r':
	case '\n':
		eol = 1;
		if (*(end+1)=='\r' || *(end+1)=='\n') {
			rsz++;
		}
		/* let through */
	case ',':
		rsz++;
		*end = 0;
	case '\0':
		/* do nothing */
		break;
	default:
		berr("Unexpected character: '%c' (%#X)", *end, *end);
		errno = EINVAL;
		return NULL;
	}

	/* eliminate trailing spaces */
	end--;
	while (end >= str && isspace(*end)) {
		*end = 0;
		end--;
	}

	/* eliminating leading spaes */
	while (*str && isspace(*str)) {
		str++;
		sz--;
		rsz--;
	}

	if (nl) {
		r++;
		c = 0;
	}

	ret = str;

	/* prepare for the next call */
	str = str + rsz;
	sz -= rsz;
	*row = r;
	*col = c;

	if (eol) {
		/* reaching End of Line, preparing r/c number
		 * for the next one */
		r++;
		c = 0;
	} else {
		c++;
	}

	return ret;
}

static
void extract_metric_routine()
{
	struct barray *bin_array = NULL;
	int rc;
	int idx;
	int row, col;
	int len;
	int i, n;
	time_t ts = 0;
	uint32_t comp_id;
	uint32_t *count_p;
	double value;
	const char *str;
	struct bassocimg_pixel pxl = {.count = 1};
	struct bassocimgbin *bin;
	struct bhash_entry *hent;
	struct bassocimg *img;
	struct bdbstr *bdbstr;
	struct barray *count_buff;
	uint32_t spp = conf_handle->conf->spp;
	uint32_t npp = conf_handle->conf->npp;

	bdbstr = bdbstr_new(256);
	if (!bdbstr) {
		berr("Not enough memory");
		exit(-1);
	}

	bin_array = barray_alloc(sizeof(void*), 1024);
	if (!bin_array) {
		berr("Not enough memory");
		exit(-1);
	}

	/* Headers */
	str = __get_metric_input(&row, &col);
	if (!str) {
		/* no data ... just return */
		return;
	}
	while (row==0) {
		len = strlen(str);
		hent = bhash_entry_get(metric2imgbin, str, len);
		if (hent) {
			bin = (void*)hent->value;
		} else {
			bin = NULL;
		}
		rc = barray_set(bin_array, col, &bin);
		if (rc) {
			berr("barray_set(), error, rc: %d", rc);
			exit(-1);
		}

		/* next cell */
		str = __get_metric_input(&row, &col);
		if (!str) {
			berr("__get_metric_input() error(%d): %m", errno);
			exit(-1);
		}
	}

	/* str contains data */
	while (str) {
		len = 0;
		switch (col) {
		case 0:
			/* time stamp */
			ts = pxl.sec;
			sscanf(str, "%lu%n", &pxl.sec, &len);
			pxl.sec /= spp;
			pxl.sec *= spp;
			if (ts != pxl.sec) {
				/* time changed, flush pixels here. */
				n = barray_get_len(bin_array);
				for (i = 2; i < n; i++) {
					bin = NULL;
					barray_get(bin_array, i, &bin);
					if (!bin)
						continue;
					rc = bassocimgbin_flush(bin, ts);
					assert(rc == 0);
				}
			}
			break;
		case 1:
			/* comp_id */
			sscanf(str, "%lu%n", &pxl.comp_id, &len);
			pxl.comp_id /= npp;
			pxl.comp_id *= npp;
			break;
		default:
			sscanf(str, "%lf%n", &value, &len);
		}

		if (!len) {
			/* bad input */
			berr("Expecting a number, but got: %s", str);
			exit(-1);
		}

		if (col < 2)
			goto next;

		barray_get(bin_array, col, &bin);

		if (!bin)
			goto next;

		idx = bassocimgbin_getbinidx(bin, value);
		assert(idx >= 0);

		if (!bin->bin[idx].img) {
			rc = bassocimgbin_open(bin, idx, bdbstr);
			if (rc) {
				berr("bassocimgbin_open() error,"
						" rc: %d", rc);
				exit(-1);
			}
		}
		count_buff = bin->bin[idx].count_buff;

		count_p = (void*)barray_get(count_buff, pxl.comp_id, NULL);
		if (!count_p || !*count_p) {
			uint32_t c = 1;
			rc = barray_set(count_buff, pxl.comp_id, &c);
			if (rc) {
				berr("Out of memory");
				exit(-1);
			}
		} else {
			(*count_p)++;
		}

		/* next entry */
	next:
		str = __get_metric_input(&row, &col);
		if (!str && errno != ENOENT) {
			/* error in the input */
			exit(-1);
		}
	}
	/* flush the last row */
	n = barray_get_len(bin_array);
	for (i = 2; i < n; i++) {
		bin = NULL;
		barray_get(bin_array, i, &bin);
		if (!bin)
			continue;
		rc = bassocimgbin_flush(bin, ts);
		assert(rc == 0);
	}
	assert(rc == 0);
	bdbstr_free(bdbstr);
}

static
void extract_routine()
{
	ptn2imglist = bhash_new(65521, 11, NULL);
	if (!ptn2imglist) {
		berr("bhash_new() error(%d): %m", errno);
		exit(-1);
	}

	metric2imgbin = bhash_new(65521, 11, NULL);
	if (!metric2imgbin) {
		berr("bhash_new() error(%d): %m", errno);
		exit(-1);
	}

	process_recpies_routine();

	if (store_path)
		extract_bstore_routine();

	if (enable_metric_stream)
		extract_metric_routine();
}

void info_routine()
{
	struct bdstr *bdstr;
	wordexp_t wexp;
	int rc, i;
	struct bmhash_iter *itr;
	struct bassoc_conf *conf = conf_handle->conf;
	printf("Configuration:\n");
	printf("\tseconds per pixel: %d\n", spp);
	printf("\tnodes per pixel: %d\n", npp);

	itr = bmhash_iter_new(img_cache->ev2img);
	if (!itr) {
		berror("bmhash_iter_new()");
		exit(-1);
	}
	printf("Images:\n");
	rc = bmhash_iter_begin(itr);
	while (rc == 0) {
		struct bmhash_entry *ent = bmhash_iter_entry(itr);
		printf("\t%.*s\n", ent->key.blen, ent->key.cstr);
		rc = bmhash_iter_next(itr);
	}
	bmhash_iter_free(itr);
}

static inline
void bassoc_rule_free(struct bassoc_rule *rule)
{
	free(rule);
}

static inline
struct bassoc_rule *bassoc_rule_new()
{
	return calloc(1, sizeof(struct bassoc_rule));
}

int bassoc_rule_q_init(struct bassoc_rule_q *q)
{
	int i, rc;
	struct bdstr *bdstr;

	q->subq[0].refcount = 0;
	q->subq[1].refcount = 0;
	q->subq[0].bmq = NULL;
	q->subq[1].bmq = NULL;

	bdstr = bdstr_new(4096);
	if (!bdstr) {
		rc = ENOENT;
		goto out;
	}
	rc = pthread_mutex_init(&q->mutex, NULL);
	if (rc)
		goto out;
	rc = pthread_cond_init(&q->cond, NULL);
	if (rc)
		goto out;
	for (i = 0; i < 2; i++) {
		bdstr_reset(bdstr);
		rc = bdstr_append_printf(bdstr, "%s/q%d", workspace_path, i);
		if (rc)
			goto out;
		q->subq[i].bmq = bmqueue_open(bdstr->str, sizeof(struct bassoc_rule), 1);
		if (!q->subq[i].bmq)
			goto out;
	}
	q->current_subq = &q->subq[0];
	q->next_subq = &q->subq[1];
	q->state = BASSOC_RULE_Q_STATE_ACTIVE;
	goto out;
err0:
	for (i = 0; i < 2; i++) {
		if (q->subq[i].bmq)
			bmqueue_close(q->subq[i].bmq);
	}
out:
	if (bdstr)
		bdstr_free(bdstr);
	return rc;
}

void bassoc_rule_add(struct bassoc_rule_q *q, struct bassoc_rule *r)
{
	int rc;
	pthread_mutex_lock(&q->mutex);
	rc = bmqueue_enqueue(q->next_subq->bmq, &r->qelm);
	if (rc) {
		berror("bmqueue_enqueue()");
		goto out;
	}
	q->next_subq->refcount++;
	pthread_cond_signal(&q->cond);
out:
	pthread_mutex_unlock(&q->mutex);
}

void bassoc_rule_put(struct bassoc_rule_q *q, struct bassoc_rule *rule)
{
	pthread_mutex_lock(&q->mutex);
	bmqueue_elm_put(&rule->qelm);
	q->current_subq->refcount--;
	if (!q->current_subq->refcount) {
		/* Done for that level */
		q->state = BASSOC_RULE_Q_STATE_LVL_DONE;
		pthread_cond_broadcast(&q->cond);
	}
	pthread_mutex_unlock(&q->mutex);
}

struct bassoc_rule *bassoc_rule_get(struct bassoc_rule_q *q)
{
	struct bassoc_rule *r;
	void *tmp;
	uint64_t refcount_tmp;
	pthread_mutex_lock(&q->mutex);
	if (q->state == BASSOC_RULE_Q_STATE_DONE) {
		r = NULL;
		goto out;
	}

loop:
	switch (q->state) {
	case BASSOC_RULE_Q_STATE_DONE:
		r = NULL;
		goto out;
	case BASSOC_RULE_Q_STATE_LVL_DONE:
		tmp = q->next_subq;
		q->next_subq = q->current_subq;
		q->current_subq = tmp;
		r = (void*)bmqueue_dequeue_nonblock(q->current_subq->bmq);
		if (r) {
			q->state = BASSOC_RULE_Q_STATE_ACTIVE;
		} else {
			q->state = BASSOC_RULE_Q_STATE_DONE;
		}
		pthread_cond_broadcast(&q->cond);
		goto out;
	case BASSOC_RULE_Q_STATE_ACTIVE:
		r = (void*)bmqueue_dequeue_nonblock(q->current_subq->bmq);
		if (!r) {
			pthread_cond_wait(&q->cond, &q->mutex);
			goto loop;
		}
		break;
	}
out:
	pthread_mutex_unlock(&q->mutex);
	return r;
}

int bassoc_rule_q_start(struct bassoc_rule_q *q)
{
	void *tmp;
	int rc = 0;
	pthread_mutex_lock(&q->mutex);
	if (q->current_subq->refcount) {
		rc = 0;
		goto out;
	}
	if (!q->next_subq->refcount) {
		rc = ENOENT;
		goto out;
	}

	tmp = q->current_subq;
	q->current_subq = q->next_subq;
	q->next_subq = tmp;

out:
	pthread_mutex_unlock(&q->mutex);
	return 0;
}

void bassoc_rule_print(struct bassoc_rule *rule, const char *prefix)
{
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int i;
	struct bassocimg *img;
	bassocimg_hdr_t hdr;
	pthread_mutex_lock(&mutex);
	fprintf(rule_out, "%s: (%lf, %lf) {", prefix, rule->conf, rule->sig);
	for (i = 1; i < rule->formula_len; i++) {
		img = NULL;
		barray_get(images, rule->formula[i], &img);
		assert(img);
		if (i>1)
			fprintf(rule_out, ",");
		hdr = BASSOCIMG_HDR(img);
		fprintf(rule_out, "%.*s", hdr->name.blen, hdr->name.cstr);
	}
	img = NULL;
	barray_get(target_images, rule->formula[0], &img);
	assert(img);
	hdr = BASSOCIMG_HDR(img);
	fprintf(rule_out, "}->{%.*s}\n", hdr->name.blen, hdr->name.cstr);
	pthread_mutex_unlock(&mutex);
}

void bassoc_rule_debug(struct bassoc_rule *rule, const char *prefix)
{
	if (blog_get_level() > BLOG_LV_DEBUG)
		return;
	bassoc_rule_print(rule, prefix);
}

struct bassoc_rule_index *bassoc_rule_index_new(size_t hash_size)
{
	struct bassoc_rule_index *index = calloc(1, sizeof(*index));
	if (!index)
		goto out;
	index->hash = bhash_new(hash_size, 11, NULL);
	if (!index->hash)
		goto err0;
	pthread_mutex_init(&index->mutex, NULL);

	goto out;
err0:
	free(index);
	index = NULL;
out:
	return index;
}

static
void __formula_begin(const char *formula, const char **a, const char **b)
{
	*a = formula;
	*b = strchr(formula, ',');
	if (!*b)
		*b = (*a) + strlen(*a);
}

static
int __next_in_formula(const char **a, const char **b)
{
	if (!**b)
		return ENOENT;
	*a = *b+1;
	*b = strchr(*a, ',');
	if (!*b)
		*b = *a + (strlen(*a));
	return 0;
}

int bassoc_rule_index_add(struct bassoc_rule_index *index, struct bassoc_rule *rule)
{
	int rc = 0;
	int key[2];
	struct bassoc_rule *r = bassoc_rule_new();
	int i;

	if (!r)
		return ENOMEM;

	key[0] = rule->formula[0];

	for (i = 1; i < rule->formula_len; i++) {
		key[1] = rule->formula[i];
		struct bassoc_rule_index_entry *ient = malloc(sizeof(*ient));
		if (!ient) {
			rc = ENOMEM;
			goto out;
		}
		ient->rule = r;
		pthread_mutex_lock(&index->mutex);
		struct bhash_entry *hent = bhash_entry_get(index->hash,
						(char*)key, sizeof(key));
		if (!hent) {
			hent = bhash_entry_set(index->hash, (char*)key,
							sizeof(key), 0);
			if (!hent) {
				rc = ENOMEM;
				pthread_mutex_unlock(&index->mutex);
				goto out;
			}
		}

		struct bassoc_rule_index_list *l = (void*)&hent->value;
		LIST_INSERT_HEAD(l, ient, entry);
		pthread_mutex_unlock(&index->mutex);
	}

out:
	return rc;
}

struct bassoc_rule_index_entry *
bassoc_rule_index_get(struct bassoc_rule_index *index, const char *key,
								size_t keylen)
{
	struct bhash_entry *hent;
	struct bassoc_rule_index_list *list;
	struct bassoc_rule_index_entry *ient = NULL;

	pthread_mutex_lock(&index->mutex);
	hent = bhash_entry_get(index->hash, key, keylen);
	if (!hent)
		goto out;
	list = (void*)&hent->value;
	ient = LIST_FIRST(list);
out:
	pthread_mutex_unlock(&index->mutex);
	return ient;
}

int handle_target(const char *tname)
{
	int rc = 0;
	int idx;
	struct bassocimg *img = NULL;
	struct bassocimg *timg = NULL;
	struct bassoc_rule rule;
	struct bdbstr *bdbstr = NULL;
	struct bhash_entry *hent;

	bdbstr = bdbstr_new(PATH_MAX);
	if (!bdbstr) {
		rc = ENOMEM;
		goto out;
	}

	bdbstr_reset(bdbstr);
	rc = bdbstr_append_printf(bdbstr, "%s", tname);
	if(rc) {
		goto out;
	}
	img = bassocimg_cache_get_img(img_cache, bdbstr->bstr, 0);
	if (!img) {
		rc = errno;
		goto out;
	}
	hent = bhash_entry_get(target_images_hash, tname, strlen(tname));
	if (hent) {
		/* the target has been processed in the current session. */
		rc = 0;
		goto out;
	}
	bdbstr_reset(bdbstr);
	rc = bdbstr_append_printf(bdbstr, "%s%+d", tname, offset);
	if(rc) {
		goto out;
	}
	timg = bassocimg_cache_get_img(target_cache, bdbstr->bstr, 1);
	if (!timg) {
		berr("Cannot get/create composite image: %s, err(%d): %m",
						bdbstr->bstr->cstr, errno);
		rc = errno;
		goto out;
	}

	hent = bhash_entry_set(target_images_hash, tname, strlen(tname), (uint64_t)timg);
	if (!hent) {
		berror("bhash_entry_set()");
		rc = errno;
		goto out;
	}

	rc = bassocimg_shift_ts(img, offset * conf_handle->conf->spp, timg);
	if (rc) {
		berr("bassocimg_shift_ts('%.*s', %d, '%.*s')"
				" failed, rc: %d",
				BASSOCIMG_HDR(img)->name.blen,
				BASSOCIMG_HDR(img)->name.cstr,
				offset * conf_handle->conf->spp,
				BASSOCIMG_HDR(timg)->name.blen,
				BASSOCIMG_HDR(timg)->name.cstr,
				rc);
		goto out;
	}

enqueue:

	barray_append(target_images, &timg);
	idx = barray_get_len(target_images) - 1;

	rule.formula_len = 1;
	rule.formula[0] = idx;
	bassoc_rule_add(&rule_q, &rule);

out:
	if (bdbstr)
		bdbstr_free(bdbstr);
	return rc;
}

int handle_mine_target_line(char *line, void *ctxt)
{
	return handle_target(line);
}

static
void handle_mine_target_file()
{
	int rc;
	if (!mine_target_file_path)
		return;

	rc = bprocess_file_by_line_w_comment(mine_target_file_path,
					handle_mine_target_line, NULL);
	if (rc) {
		berr("Error processing target file: %s. %s",
					mine_target_file_path, strerror(rc));
		exit(-1);
	}
}

static
void init_images_routine()
{
	int rc, i;
	int idx;
	struct bassocimg *img;
	const char *name;
	wordexp_t wexp;

	images = barray_alloc(sizeof(void*), 1024);
	if (!images) {
		berr("Not enough memory");
		exit(-1);
	}

	target_images = barray_alloc(sizeof(void*), 1024);
	if (!target_images) {
		berr("Not enough memory");
		exit(-1);
	}

	target_images_hash = bhash_new(65539, 11, NULL);
	if (!target_images_hash) {
		berr("Not enough memory");
		exit(-1);
	}

	struct bmhash_iter *itr = bmhash_iter_new(img_cache->ev2img);
	if (!itr) {
		berror("bmhash_iter_new()");
		exit(-1);
	}

	struct bmhash_entry *hent;
	rc = bmhash_iter_begin(itr);
	i = 0;

	while (rc == 0) {
		hent = bmhash_iter_entry(itr);
		img = bassocimg_cache_get_img(img_cache, &hent->key, 0);
		if (!img) {
			berr("Cannot open image: %.*s, errno: %d",
				hent->key.blen, hent->key.cstr, errno);
			exit(-1);
		}
		rc = barray_set(images, i, &img);
		if (rc) {
			berr("barray_set() error, rc: %d", rc);
			exit(-1);
		}
		rc = bmhash_iter_next(itr);
		i++;
	}
}

void init_target_images_routine()
{
	int i, n, rc;
	struct bassocimg *img;
	struct bassocimg *timg;
	const char *tname;

	handle_mine_target_file();

	n = barray_get_len(cli_targets);
	for (i = 0; i < n; i++) {
		barray_get(cli_targets, i, &tname);
		rc = handle_target(tname);
		if (rc) {
			berr("Error processing target: %s. %s", tname,
							strerror(rc));
			exit(-1);
		}
	}
}

#define MINER_CTXT_STACK_SZ (BASSOC_MAX_RULE_DEPTH)

struct miner_ctxt {
	struct bdstr *bdstr;
	int key[2];

	/* Image-Recipe stack */
	/* The additional +2 are the space for miner operation */
	struct bassocimg *img[MINER_CTXT_STACK_SZ + 2];
	int recipe[MINER_CTXT_STACK_SZ];
	int stack_sz;

	struct bassocimg_cache *cache;
};

struct miner_ctxt *miner_init(int thread_number)
{
	int i, rc;
	struct miner_ctxt *ctxt = calloc(1, sizeof(*ctxt));
	if (!ctxt)
		goto out;

	ctxt->bdstr = bdstr_new(PATH_MAX);
	if (!ctxt->bdstr)
		goto err0;

	bdstr_reset(ctxt->bdstr);
	rc = bdstr_append_printf(ctxt->bdstr, "%s/miner-%d.cache",
						workspace_path, thread_number);
	if (rc)
		goto err0;

	ctxt->cache = bassocimg_cache_open(ctxt->bdstr->str, 1);
	if (!ctxt->cache) {
		berror("bassocimg_cache_open()");
		goto err0;
	}

	char buff[128];
	struct bstr *bstr = (void*)buff;
	int alen = sizeof(buff) - sizeof(*bstr);

	for (i = 1; i < MINER_CTXT_STACK_SZ + 2; i++) {
		/* img[0] doesn't need stack cache */
		bstr->blen = snprintf(bstr->cstr, alen, "%d", i);
		assert(bstr->blen < alen);
		ctxt->img[i] = bassocimg_cache_get_img(ctxt->cache, bstr, 1);
		if (!ctxt->img[i]) {
			berr("cannot open/create img: %s", ctxt->bdstr->str);
			goto err1;
		}
	}

	goto out;

err1:
	for (i = 1; i < MINER_CTXT_STACK_SZ + 2; i++) {
		if (ctxt->img[i])
			bassocimg_put(ctxt->img[i]);
	}

	bdstr_free(ctxt->bdstr);
err0:
	free(ctxt);
out:
	return ctxt;
}

int miner_add_stack_img(struct miner_ctxt *ctxt, struct bassocimg *img,
								int img_idx)
{
	int i = ctxt->stack_sz;
	int rc;
	if (i >= MINER_CTXT_STACK_SZ)
		return ENOMEM;
	ctxt->stack_sz++;
	ctxt->recipe[i] = img_idx;
	if (i == 0) {
		ctxt->img[0] = img;
		return 0;
	}
	return bassocimg_intersect(ctxt->img[i-1], img, ctxt->img[i]);
}

/*
 * Test whether \c r0 is more general than \c r1.
 * In other words, test whether \c r0 is a subset of \c r1.
 */
static
int bassoc_rule_general(struct bassoc_rule *r0, struct bassoc_rule *r1)
{
	int i0, i1;
	if (r0->formula[0] != r1->formula[0])
		/* Different target */
		return 0;
	i0 = i1 = 1;
	while (i0 < r0->formula_len && i1 < r1->formula_len) {
		if (r0->formula[i0] < r1->formula[i1])
			return 0;
		if (r0->formula[i0] == r1->formula[i1])
			i0++;
		i1++;
	}
	return i0 == r0->formula_len;
}

void *miner_proc(void *arg)
{
	int thread_number = (uint64_t)arg;
	int i, n, rc;
	int idx;
	int fidx;
	int sidx;
	int tidx;
	int rlen;
	const char *s;
	const char *t;
	const char *target;
	struct bhash_entry *hent;
	/* suppose rule := Ax->z */
	struct bassoc_rule *rule;
	struct bassocimg *timg; /* target image (z) */
	struct bassocimg *aimg; /* antecedent image (Ax) */
	struct bassocimg *bimg; /* base of antecedent image (A) */
	struct bassocimg *cimg; /* consequence+antecedent image (Axz) */
	struct bassocimg *img;

	int next_idx;

	bassocimg_hdr_t ahdr, bhdr, chdr, thdr;
	int *formula; /* formula[0] = tgt, formula[x] = event */
	size_t formula_len;

	struct miner_ctxt *ctxt;

	ctxt = miner_init(thread_number);

	if (!ctxt) {
		/* Error details should be printed within miner_init() */
		return 0;
	}

	bdebug("miner %d: beginning ...", thread_number);

	n = barray_get_len(images);

	cimg = ctxt->img[MINER_CTXT_STACK_SZ + 1];

loop:
	rule = bassoc_rule_get(&rule_q);
	if (!rule ) {
		/* DONE */
		goto out;
	}

	formula = rule->formula;
	formula_len = rule->formula_len;
	tidx = formula[0];
	barray_get(target_images, tidx, &timg);

	sidx = 0;
	fidx = 1;
	while (sidx < ctxt->stack_sz && fidx < formula_len) {
		if (ctxt->recipe[sidx] != formula[fidx])
			break;
		sidx++;
		fidx++;
	}

	/* we can reuse what in the stack, up to sidx - 1 */
	ctxt->stack_sz = sidx;

	while (fidx < formula_len) {
		img = NULL;
		barray_get(images, formula[fidx], &img);
		if (!img) {
			berr("Cannot get image: %.*s", (int)(t - s), s);
			assert(0);
		}
		rc = miner_add_stack_img(ctxt, img, formula[fidx]);
		fidx++;
	}
	/* Now, the top-of-stack is the antecedent image, denoted by (B) */

	bimg = (ctxt->stack_sz)?(ctxt->img[ctxt->stack_sz - 1]):(NULL);
	aimg = ctxt->img[MINER_CTXT_STACK_SZ];

	if (rule->formula_len > 1) {
		next_idx = 1 + rule->formula[rule->formula_len-1];
	} else {
		next_idx = 0;
	}

	/* reuse the `rule` in candidate discovery */
	assert(rule->formula_len < BASSOC_MAX_RULE_DEPTH + 1);
	rule->formula_len++;

	for (i = next_idx; i < n; i++) {
		/* Expanding rule candidates to discover rules */
		/* i.e., considering (B)(i)->(t) */
		/* note: (A) = (B)(i) */
		barray_get(images, i, &img);

		/* construct rule candidate */
		rule->formula[rule->formula_len-1] = i;

		ctxt->key[0] = tidx;
		ctxt->key[1] = i;

		struct bassoc_rule_index_entry *rent = bassoc_rule_index_get(
						rule_index, (char*)ctxt->key,
						sizeof(ctxt->key));

		/* General rule bound */
		while (rent) {
			if (bassoc_rule_general(rent->rule, rule)) {
				bassoc_rule_debug(rule, "general bounded");
				goto bound;
			}
			rent = LIST_NEXT(rent, entry);
		}

		/* intersect ... */
		if (bimg) {
			rc = bassocimg_intersect(bimg, img, aimg);
			assert(rc == 0);
		} else {
			aimg = img;
		}
		/* Now, (A) = (B)(i) */

		/* note: (C) = (A)(t) = (B)(i)(t) */
		rc = bassocimg_intersect(aimg, timg, cimg);
		assert(rc == 0);

		/* Recall:
		 *   we're considering rule (B)(i)->(t)
		 *     (A) = (B)(i)
		 *     (C) = (B)(i)(t)
		 */

		uint64_t count_a, count_b, count_c, count_t;

		if (blackwhite) {
			count_a = bassocimg_get_pixel_len(aimg);
			count_b = (bimg)?(bassocimg_get_pixel_len(bimg)):(0);
			count_c = bassocimg_get_pixel_len(cimg);
			count_t = bassocimg_get_pixel_len(timg);
		} else {
			count_a = BASSOCIMG_HDR(aimg)->count;
			count_b = (bimg)?(BASSOCIMG_HDR(bimg)->count):(0);
			count_c = BASSOCIMG_HDR(cimg)->count;
			count_t = BASSOCIMG_HDR(timg)->count;
		}

		/* empty-set check */
		if (!count_a || !count_c) {
			bassoc_rule_debug(rule, "empty-set bounded");
			goto bound;
		}

		/* calculate confidence, significance */
		rule->conf = count_c / (double)count_a;
		rule->sig = count_c / (double)count_t;

		if (rule->sig < significance) {
			/* Significance bound */
			bassoc_rule_debug(rule, "significance bounded");
			goto bound;
		}

		if (rule->conf > confidence) {
			/* This is a rule, no need to expand more */
			bassoc_rule_print(rule, "rule");
			bassoc_rule_index_add(rule_index, rule);
			goto term;
		}

		if (bimg && (count_b - count_a) / (double)(count_b) < difference) {
			/* Difference bound */
			bassoc_rule_debug(rule, "difference bounded");
			goto bound;
		}

		if (rule->formula_len == 1 + BASSOC_MAX_RULE_DEPTH) {
			bassoc_rule_debug(rule, "rule depth bound");
			goto bound;
		}

		/* Good candidate, add to the queue */
		bassoc_rule_add(&rule_q, rule);
		bassoc_rule_debug(rule, "valid candidate");

		continue;

	bound:
	term:
		continue;
	}

end:
	/* done with the rule, put it down */
	bassoc_rule_put(&rule_q, rule);
	goto loop;

out:
	bdebug("miner %d: exiting ...", thread_number);
	return NULL;
}

static
void mine_routine()
{
	int i, rc;
	/* Initialize rule mining queue */
	rc = bassoc_rule_q_init(&rule_q);
	if (rc) {
		berr("bassoc_rule_q_init() error, rc: %d", rc);
		exit(-1);
	}

	init_images_routine();
	init_target_images_routine();

	if (miner_threads - 1) {
		miner = calloc(sizeof(pthread_t), miner_threads - 1);
		if (!miner) {
			berr("Out of memory");
			exit(-1);
		}
	}

	rule_index = bassoc_rule_index_new(65521);
	if (!rule_index) {
		berr("Out of memory");
		exit(-1);
	}

	/* start the queue before starting the threads */
	bassoc_rule_q_start(&rule_q);

	binfo("Mining ...");

	for (i = 0; i < miner_threads - 1; i++) {
		rc = pthread_create(&miner[i], NULL, miner_proc, (void*)(uint64_t)i);
		if (rc) {
			berr("pthread_create() error, rc: %d", rc);
			exit(-1);
		}
	}

	miner_proc((void*)(uint64_t)(miner_threads - 1));

	for (i = 0; i < miner_threads - 1; i++) {
		pthread_join(miner[i], NULL);
	}
}

void init() {
	cli_recipe = barray_alloc(sizeof(void*), 1024);
	if (!cli_recipe) {
		berr("Out of memory");
		exit(-1);
	}
	cli_targets = barray_alloc(sizeof(void*), 1024);
	if (!cli_targets) {
		berr("Out of memory");
		exit(-1);
	}
	blog_set_level_str("INFO");
}

int main(int argc, char **argv)
{
	init();
	handle_args(argc, argv);
	void (*(routine[]))(void) = {
		[RUN_MODE_CREATE]   =  create_routine,
		[RUN_MODE_EXTRACT]  =  extract_routine,
		[RUN_MODE_INFO]     =  info_routine,
		[RUN_MODE_MINE]     =  mine_routine,
	};
	switch (run_mode_flag) {
	case RUN_MODE_EXTRACT:
	case RUN_MODE_INFO:
	case RUN_MODE_MINE:
		open_bassoc_conf_routine();
		open_cache_routine();
		/* let-through */
	case RUN_MODE_CREATE:
		routine[run_mode_flag]();
		break;
	default:
		berr("Cannot determine run mode.");
	}
	return 0;
}
