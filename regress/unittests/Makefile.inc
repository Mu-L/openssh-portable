#	$OpenBSD: Makefile.inc,v 1.17 2025/04/15 04:00:42 djm Exp $

.include <bsd.own.mk>
.include <bsd.obj.mk>

# User-settable options
UNITTEST_FAST?= no	# Skip slow tests (e.g. less intensive fuzzing).
UNITTEST_SLOW?= no	# Include slower tests (e.g. more intensive fuzzing).
UNITTEST_VERBOSE?= no	# Verbose test output (inc. per-test names).
UNITTEST_BENCHMARK?= no	# Run unit tests in benchmarking mode.
UNITTEST_BENCH_DETAIL?=no # Detailed benchmark statistics.
UNITTEST_BENCH_ONLY?=	# Run only these benchmarks

MALLOC_OPTIONS?=	CFGJRSUX
TEST_ENV?=		MALLOC_OPTIONS=${MALLOC_OPTIONS}

# XXX detect from ssh binary?
OPENSSL?=	yes
DSAKEY?=	yes

.if (${DSAKEY:L} == "yes")
CFLAGS+=	-DWITH_DSA
.endif

.if (${OPENSSL:L} == "yes")
CFLAGS+=	-DWITH_OPENSSL
.endif

# enable warnings
WARNINGS=Yes

DEBUG=-g
CFLAGS+=	-fstack-protector-all
CDIAGFLAGS=	-Wall
CDIAGFLAGS+=	-Wextra
CDIAGFLAGS+=	-Werror
CDIAGFLAGS+=	-Wchar-subscripts
CDIAGFLAGS+=	-Wcomment
CDIAGFLAGS+=	-Wformat
CDIAGFLAGS+=	-Wformat-security
CDIAGFLAGS+=	-Wimplicit
CDIAGFLAGS+=	-Winline
CDIAGFLAGS+=	-Wmissing-declarations
CDIAGFLAGS+=	-Wmissing-prototypes
CDIAGFLAGS+=	-Wparentheses
CDIAGFLAGS+=	-Wpointer-arith
CDIAGFLAGS+=	-Wreturn-type
CDIAGFLAGS+=	-Wshadow
CDIAGFLAGS+=	-Wsign-compare
CDIAGFLAGS+=	-Wstrict-aliasing
CDIAGFLAGS+=	-Wstrict-prototypes
CDIAGFLAGS+=	-Wswitch
CDIAGFLAGS+=	-Wtrigraphs
CDIAGFLAGS+=	-Wuninitialized
CDIAGFLAGS+=	-Wunused
CDIAGFLAGS+=	-Wno-unused-parameter
.if ${COMPILER_VERSION:L} != "gcc3"
CDIAGFLAGS+=	-Wold-style-definition
.endif

SSHREL=../../../../../usr.bin/ssh

CFLAGS+=-I${.CURDIR}/../test_helper -I${.CURDIR}/${SSHREL}

.if exists(${.CURDIR}/../test_helper/${__objdir})
LDADD+=-L${.CURDIR}/../test_helper/${__objdir} -ltest_helper
DPADD+=${.CURDIR}/../test_helper/${__objdir}/libtest_helper.a
.else
LDADD+=-L${.CURDIR}/../test_helper -ltest_helper
DPADD+=${.CURDIR}/../test_helper/libtest_helper.a
.endif

.PATH: ${.CURDIR}/${SSHREL}

LDADD+= -lutil -lm
DPADD+= ${LIBUTIL} ${LIBM}

.if (${OPENSSL:L} == "yes")
LDADD+= -lcrypto
DPADD+= ${LIBCRYPTO}
.endif

LDADD+= -lfido2 -lcbor -lusbhid
DPADD+= ${LIBFIDO2} ${LIBCBOR} ${LIBUSBHID}

UNITTEST_ARGS?=

.if (${UNITTEST_VERBOSE:L:R} != "no")
UNITTEST_ARGS+= -v
.endif
.if (${UNITTEST_FAST:L:R} != "no")
UNITTEST_ARGS+= -f
.elif (${UNITTEST_SLOW:L:R} != "no")
UNITTEST_ARGS+= -F
.endif

.if (${UNITTEST_BENCHMARK:L:R} != "no")
UNITTEST_ARGS+= -b
.endif
.if (${UNITTEST_BENCH_DETAIL:L:R} != "no")
UNITTEST_ARGS+= -B
.endif
.if (${UNITTEST_BENCH_ONLY:L} != "")
UNITTEST_ARGS+= -O "${UNITTEST_BENCH_ONLY}"
.endif
