/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx       = 3;   /* border pixel of windows */
static const unsigned int snap           = 32;  /* snap pixel */
static const int scalepreview            = 4;        /* Tag preview scaling */
static const unsigned int gappih         = 20;  /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;  /* vert inner gap between windows */
static const unsigned int gappoh         = 10;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 30;  /* vert outer gap between windows and screen edge */
static const int smartgaps_fact          = 1;   /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
static const char autostartblocksh[]     = "autostart_blocking.sh";
static const char autostartsh[]          = "autostart.sh";
static const char dwmdir[]               = "dwm";
static const char localshare[]           = ".local/share";
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
#define ICONSIZE 20    /* icon size */
#define ICONSPACING 5  /* space between icon and title */
static int floatposgrid_x                = 5;  /* float grid columns */
static int floatposgrid_y                = 5;  /* float grid rows */
/* Status is to be shown on: -1 (all monitors), 0 (a specific monitor by index), 'A' (active monitor) */
static const int statusmon               = -1;
static const int horizpadbar             = 10;   /* horizontal padding for statusbar */
static const int vertpadbar              = 10;   /* vertical padding for statusbar */
static const char buttonbar[]            = "  ";
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int showsystray             = 1;   /* 0 means no systray */
static const unsigned int systrayiconsize = 16; /* systray icon size in px */
static const unsigned int ulinepad = 10;         /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke  = 2;     /* thickness / height of the underline */
static const unsigned int ulinevoffset = -2;     /* how far above the bottom of the bar the line should appear */
static const int ulineall = 0;                  /* 1 to show underline on all tags, 0 for just the active ones */



/* Indicators: see patch/bar_indicators.h for options */
static int tagindicatortype              = INDICATOR_TOP_LEFT_SQUARE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;
static void (*bartabmonfns[])(Monitor *) = { NULL /* , customlayoutfn */ };
static const char *fonts[]               = { "Iosevka Nerd Font:size=12" };
static const char dmenufont[]            = "monospace:size=10";

static char c000000[]                    = "#000000"; // placeholder value

static char normfgcolor[]                = "#89b4fa";
static char normbgcolor[]                = "#181825";
static char normbordercolor[]            = "#181825";
static char normfloatcolor[]             = "#db8fd9";

static char selfgcolor[]                 = "#181825";
static char selbgcolor[]                 = "#cba6f7";
static char selbordercolor[]             = "#cba6f7";
static char selfloatcolor[]              = "#cba6f7";

static char titlenormfgcolor[]           = "#eeeeee";
static char titlenormbgcolor[]           = "#181825";
static char titlenormbordercolor[]       = "#181825";
static char titlenormfloatcolor[]        = "#181825";

static char titleselfgcolor[]            = "#eeeeee";
static char titleselbgcolor[]            = "#181825";
static char titleselbordercolor[]        = "#181825";
static char titleselfloatcolor[]         = "#181825";

static char tagsnormfgcolor[]            = "#181825";
static char tagsnormbgcolor[]            = "#cba6f7";
static char tagsnormbordercolor[]        = "#444444";
static char tagsnormfloatcolor[]         = "#db8fd9";

static char tagsselfgcolor[]             = "#cba6f7";
static char tagsselbgcolor[]             = "#181825";
static char tagsselbordercolor[]         = "#181825";
static char tagsselfloatcolor[]          = "#181825";

static char hidnormfgcolor[]             = "#005577";
static char hidselfgcolor[]              = "#227799";
static char hidnormbgcolor[]             = "#222222";
static char hidselbgcolor[]              = "#222222";

static char urgfgcolor[]                 = "#bbbbbb";
static char urgbgcolor[]                 = "#222222";
static char urgbordercolor[]             = "#ff0000";
static char urgfloatcolor[]              = "#db8fd9";

static char scratchselfgcolor[]          = "#FFF7D4";
static char scratchselbgcolor[]          = "#77547E";
static char scratchselbordercolor[]      = "#894B9F";
static char scratchselfloatcolor[]       = "#894B9F";

static char scratchnormfgcolor[]         = "#FFF7D4";
static char scratchnormbgcolor[]         = "#664C67";
static char scratchnormbordercolor[]     = "#77547E";
static char scratchnormfloatcolor[]      = "#77547E";



static char *colors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm]         = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor },
	[SchemeSel]          = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor },
	[SchemeTitleNorm]    = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
	[SchemeTitleSel]     = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor },
	[SchemeTagsNorm]     = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor },
	[SchemeTagsSel]      = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor },
	[SchemeHidNorm]      = { hidnormfgcolor,   hidnormbgcolor,   c000000,              c000000 },
	[SchemeHidSel]       = { hidselfgcolor,    hidselbgcolor,    c000000,              c000000 },
	[SchemeUrg]          = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor },
	[SchemeScratchSel]  = { scratchselfgcolor, scratchselbgcolor, scratchselbordercolor, scratchselfloatcolor },
	[SchemeScratchNorm] = { scratchnormfgcolor, scratchnormbgcolor, scratchnormbordercolor, scratchnormfloatcolor },
};




static const char *scratchpadcmd[] = {"s", "kitty", "--title", "scratchpad", NULL};
static const char *pulsescratch[] = {"p", "kitty", "--title", "pulsemixer", "-e", "pulsemixer", NULL};

/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char *tagicons[][NUMTAGS] =
{
	[DEFAULT_TAGS]        = { "", "", "", "", ""},
	// [ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
	// [ALT_TAGS_DECORATION] = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};


/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	WM_WINDOW_ROLE(STRING) = role
	 *	_NET_WM_WINDOW_TYPE(ATOM) = wintype
	 */
	RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
	RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
	RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
	RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
	// RULE(.class = "Gimp", .tags = 1 << 4)
  RULE(.class = "firefox", .tags = 1 << 1)
	RULE(.title = "scratchpad", .scratchkey = 's', .isfloating = 1, .floatpos = "50% 50% 900W 600H")
	RULE(.title = "pulsemixer", .scratchkey = 'p', .isfloating = 1, .floatpos = "50% 50% 900W 600H")
};

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor   bar    alignment         widthfunc                 drawfunc                clickfunc                hoverfunc                name */
	{ -1,        0,     BAR_ALIGN_LEFT,   width_stbutton,           draw_stbutton,          click_stbutton,          NULL,                    "statusbutton" },
	{ -1,        0,     BAR_ALIGN_LEFT,   width_tags,               draw_tags,              click_tags,              hover_tags,              "tags" },
	{  0,        0,     BAR_ALIGN_RIGHT,  width_systray,            draw_systray,           click_systray,           NULL,                    "systray" },
	{ -1,        0,     BAR_ALIGN_LEFT,   width_ltsymbol,           draw_ltsymbol,          click_ltsymbol,          NULL,                    "layout" },
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_status2d,           draw_status2d,          click_statuscmd,         NULL,                    "status2d" },
	// { -1,        0,     BAR_ALIGN_NONE,   width_bartabgroups,       draw_bartabgroups,      click_bartabgroups,      NULL,                    "bartabgroups" },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int nstack      = 0;    /* number of clients in primary stack area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */



static const Layout layouts[] = {
	/* symbol     arrange function, { nmaster, nstack, layout, master axis, stack axis, secondary stack axis, symbol func } */
	{ "MS Stack",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0, NULL } }, // default tile layout
 	{ "Float",      NULL,             {0} },    /* no layout function means floating behavior */
	{ "Monocle",      flextile,         { -1, -1, NO_SPLIT, MONOCLE, MONOCLE, 0, NULL } }, // monocle
	// { "|||",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // columns (col) layout
	// { ">M>",      flextile,         { -1, -1, FLOATING_MASTER, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // floating master
	{ "Deck",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, MONOCLE, 0, NULL } }, // deck
	// { "===",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // bstackhoriz
	{ "Centered MS",      flextile,         { -1, -1, SPLIT_CENTERED_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, TOP_TO_BOTTOM, NULL } }, // centeredmaster
	{ "B Stack",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // bstack
	// { "-M-",      flextile,         { -1, -1, SPLIT_CENTERED_HORIZONTAL, TOP_TO_BOTTOM, LEFT_TO_RIGHT, LEFT_TO_RIGHT, NULL } }, // centeredmaster horiz
	// { ":::",      flextile,         { -1, -1, NO_SPLIT, GAPPLESSGRID, GAPPLESSGRID, 0, NULL } }, // gappless grid
	// { "[\\]",     flextile,         { -1, -1, NO_SPLIT, DWINDLE, DWINDLE, 0, NULL } }, // fibonacci dwindle
	// { "(@)",      flextile,         { -1, -1, NO_SPLIT, SPIRAL, SPIRAL, 0, NULL } }, // fibonacci spiral
	// { "[T]",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TATAMI, 0, NULL } }, // tatami mats
	{ NULL,       NULL,             {0} },
};


/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask, KEY,      toggletag,      {.ui = 1 << TAG} }, \



/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
	"dmenu_run",
	"-m", dmenumon,
	"-fn", dmenufont,
	"-nb", normbgcolor,
	"-nf", normfgcolor,
	"-sb", selbgcolor,
	"-sf", selfgcolor,
	NULL
};
static const char *termcmd[]  = { "kitty", NULL };

/* This defines the name of the executable that handles the bar (used for signalling purposes) */
#define STATUSBAR "dwmblocks"


static const Key keys[] = {
	/* modifier                     key            function                argument */
	{ MODKEY,                       XK_p,          togglescratch,                  {.v = pulsescratch} },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$HOME/.config/rofi/launchers/launcher_switch.sh") },
	{ MODKEY,                       XK_r,      spawn,          SHCMD("$HOME/.config/rofi/launchers/launcher.sh") },
	{ MODKEY,             XK_Return,     spawn,                  {.v = termcmd } },
	{ MODKEY,           XK_BackSpace,      focusmaster,            {0} },
	{ MODKEY,                       XK_j,          focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,             {.i = -1 } },
	{ MODKEY|ShiftMask,              XK_comma,          inplacerotate,          {.i = +1 } }, // same as rotatestack
	{ MODKEY|ShiftMask,              XK_period,          inplacerotate,          {.i = -1 } }, // same as reotatestack
	{ MODKEY,                       XK_comma,          incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_period,          incnmaster,             {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,               {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,               {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,          setcfact,               {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_l,          setcfact,               {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_0,          setcfact,               {0} },
	{ MODKEY|ShiftMask,             XK_j,          movestack,              {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,          movestack,              {.i = -1 } },
	{ MODKEY|ShiftMask,                       XK_BackSpace,     zoom,                   {0} },
	{ MODKEY,                       XK_Tab,        view,                   {0} },
	{ MODKEY|ControlMask,           XK_m,          showhideclient,         {0} },
	{ MODKEY|ShiftMask,             XK_c,          killclient,             {0} },
	{ MODKEY|ShiftMask,             XK_q,          quit,                   {0} },
	{ MODKEY|ShiftMask, XK_r,          quit,                   {1} },
	{ MODKEY,             XK_F5,         xrdb,                   {.v = NULL } },
	{ MODKEY|ControlMask,           XK_Return,     mirrorlayout,           {0} },          /* flextile, flip master and stack areas */
	{ MODKEY|ShiftMask,             XK_f,      togglefloating,         {0} },
	{ MODKEY,             XK_s,          togglesticky,           {0} },
	{ MODKEY|ShiftMask,                       XK_Return,      togglescratch,          {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,              XK_t,      unfloatvisible,         {0} },
	{ MODKEY,                       XK_f,          togglefullscreen,       {0} },
	{ MODKEY,                       XK_o,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_i,     focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i, tagmon,         {.i = +1 } },

  /* Media Keys */
  {0, XF86XK_AudioLowerVolume, spawn, SHCMD("amixer set Master 5%-; pkill -RTMIN+10 dwmblocks")},
  {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("amixer set Master 5%+; pkill -RTMIN+10 dwmblocks")},
  {0, XF86XK_AudioMute, spawn, SHCMD("amixer set Master toggle; pkill -RTMIN+10 dwmblocks")},
  {0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl s 10%-")},
  {0, XF86XK_MonBrightnessUp, spawn, SHCMD("brightnessctl s 10%+")},

	/* Note that due to key limitations the below example kybindings are defined with a Mod3Mask,
	 * which is not always readily available. Refer to the patch wiki for more details. */
	/* Client position is limited to monitor window area */
	TAGKEYS(                        XK_1,                                  0)
	TAGKEYS(                        XK_2,                                  1)
	TAGKEYS(                        XK_3,                                  2)
	TAGKEYS(                        XK_4,                                  3)
	TAGKEYS(                        XK_5,                                  4)
	TAGKEYS(                        XK_6,                                  5)
	TAGKEYS(                        XK_7,                                  6)
	TAGKEYS(                        XK_8,                                  7)
	TAGKEYS(                        XK_9,                                  8)
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ ClkButton,            0,                   Button1,        spawn,          {.v = dmenucmd } },
	{ ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
	// { ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                   Button1,        togglewin,      {0} },
	// { ClkWinTitle,          0,                   Button3,        showhideclient, {0} },
	// { ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	// { ClkStatusText,        0,                   Button1,        sigstatusbar,   {.i = 1 } },
	// { ClkStatusText,        0,                   Button2,        sigstatusbar,   {.i = 2 } },
	// { ClkStatusText,        0,                   Button3,        sigstatusbar,   {.i = 3 } },
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
	/* signum       function        argument  */
	{ 1,            setlayout,      {.v = &layouts[0]} },
	{ 2,            setlayout,      {.v = &layouts[1]} },
	{ 3,            setlayout,      {.v = &layouts[2]} },
	{ 4,            setlayout,      {.v = &layouts[3]} },
	{ 5,            setlayout,      {.v = &layouts[4]} },
	{ 6,            setlayout,      {.v = &layouts[5]} },
	{ 7,            cyclelayout,    {.i = +1 }},
};

