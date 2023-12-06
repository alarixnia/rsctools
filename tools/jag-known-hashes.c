#include <stdio.h>
#include <stdlib.h>
#include "jag.h"

const char *file_names[] = {
    "badenc.txt", "fragmentsenc.txt", "hostenc.txt", "tldlist.txt",
    "badwords.txt", "boundary.txt", "entity.txt", "floor.txt", "location.txt",
    "npc.txt", "objects.txt", "prayers.txt", "projectile.txt", "roof.txt",
    "shop.txt", "spells.txt", "words.txt", "integer.dat", "string.dat",
    "index.dat", "advance.pcm", "anvil.pcm", "chisel.pcm", "click.pcm",
    "closedoor.pcm", "coins.pcm", "combat1a.pcm", "combat1b.pcm",
    "combat2a.pcm", "combat2b.pcm", "combat3a.pcm", "combat3b.pcm",
    "cooking.pcm", "death.pcm", "dropobject.pcm", "eat.pcm", "filljug.pcm",
    "fish.pcm", "foundgem.pcm", "mechanical.pcm", "mine.pcm", "mix.pcm",
    "opendoor.pcm", "outofammo.pcm", "potato.pcm", "prayeroff.pcm",
    "prayeron.pcm", "prospect.pcm", "recharge.pcm", "retreat.pcm",
    "secretdoor.pcm", "shoot.pcm", "spellfail.pcm", "spellok.pcm",
    "takeobject.pcm", "underattack.pcm", "victory.pcm", "h11p.jf", "h12b.jf",
    "h12p.jf", "h13b.jf", "h14b.jf", "h16b.jf", "h20b.jf", "h24b.jf",
    "logo.tga", "books.txt", NULL
};

const char *media_names[] = {
    "arrows", "bubble", "buttons", "compass", "corners", "crowns", "hbar",
    "hbar2", "icon", "inv1", "inv2", "projectile", "runescape", "scrollbar",
    "splat", NULL
};

const char *animation_names[] = {
    "head1", "head1a", "body1", "body1a", "legs1", "legs1a", "fhead1",
    "fhead1a", "fbody1", "fbody1a", "head2", "head2a", "head3", "head3a",
    "head4", "head4a", "chefshat", "chefshata", "apron", "aprona", "boots",
    "bootsa", "fullhelm", "fullhelma", "chainmail", "chainmaila",
    "platemailtop", "platemailtopa", "platemaillegs", "platemaillegsa",
    "leatherarmour", "leatherarmoura", "leathergloves", "leatherglovesa",
    "sword", "sworda", "fplatemailtop", "fplatemailtopa", "cape", "capea",
    "mediumhelm", "mediumhelma", "wizardsrobe", "wizardsrobea", "wizardshat",
    "wizardshata", "necklace", "necklacea", "skirt", "skirta", "squareshield",
    "squareshielda", "crossbow", "longbow", "battleaxe", "battleaxea", "mace",
    "macea", "staff", "staffa", "rat", "rata", "demon", "demona", "spider",
    "spidera", "camel", "camela", "cow", "cowa", "sheep", "unicorn", "unicorna",
    "bear", "beara", "chicken", "chickena", "skeleton", "skeletona", "skelweap",
    "skelweapa", "skelweapf", "zombie", "zombiea", "zombweap", "zombweapa",
    "zombweapf", "ghost", "ghosta", "bat", "bata", "goblin", "goblina",
    "gobweap", "gobweapa", "gobweapf", "scorpion", "scorpiona", "dragon",
    "dragona", "wolf", "wolfa", "partyhat", "partyhata", "eyepatch",
    "eyepatcha", "eyepatchf", "gasmask", "gasmaska", "spear", "speara",
    "halloweenmask", "halloweenmaska", "santahat", "santahata", "ibanstaff",
    "ibanstaffa", "souless", "soulessa", "bunnyears", "bunnyearsa",
    "saradominstaff", "saradominstaffa", "scythe", "scythea", NULL
};

const char *texture_names[] = {
    "wall", "water", "wall", "planks", "wall", "wall", "roof", "wall",
    "leafytree", "treestump", "fence", "mossy", "railings", "painting1",
    "painting2", "marble", "deadtree", "fountain", "wall", "target", "books",
    "timbered", "timbered", "mossybricks", "growingwheat", "gungywater",
    "web", "wall", "wall", "cavern", "cavern2", "lava", "pentagram",
    "mapletree", "yewtree", "helmet", "canvas", "chainmail2", "mummy",
    "jungleleaf", "jungleleaf3", "jungleleaf4", "jungleleaf5", "jungleleaf6",
    "mossybricks", "planks", "planks", "cargonet", "bark", "canvas", "canvas",
    "wall", "cavern", "cavern2", "lava", NULL
};

const char *model_names[] = {
    "tree2", "tree", "well", "table", "treestump", "ladder", "ladderdown",
    "chair", "logpile", "longtable", "throne", "range", "gravestone1",
    "gravestone2", "bigbed", "bed", "barpumps", "chestopen", "chestclosed",
    "altar", "wallpost", "supportnw", "barrel", "bench", "portrait", "candles",
    "fountain", "landscape", "mill", "counter", "market", "target", "palm2",
    "palm", "fern", "cactus", "bullrushes", "flower", "mushroom", "coffin",
    "coffin2", "woodenstairs", "woodenstairsdown", "stonestairs",
    "stonestairsdown", "woodenrailing", "marblepillar", "bookcase", "sink",
    "sworddummy", "anvil", "torcha1", "milltop", "millbase", "cart", "sacks",
    "cupboard", "metalgateclosed", "metalgateopen", "woodengateopen",
    "woodengateclosed", "signpost", "doubledoorsopen", "doubledoorsclosed",
    "henge", "dolmen", "deadtree1", "cupboardopen", "wheat", "shopsign",
    "windmillsail", "pipe&drain", "manholeclosed", "manholeopen", "wallpipe",
    "firea1", "rocks1", "rocks2", "copperrock1", "ironrock1", "tinrock1",
    "mithrilrock1", "adamiterock1", "coalrock1", "goldrock1", "clayrock1",
    "ceilingweb", "floorweb", "furnace", "madmachine", "spinningwheel",
    "leverup", "leverdown", "compost", "skulltorcha1", "chaosaltar",
    "wallshield", "wallgrill", "cauldron", "minecart", "trackbuffer",
    "trackcurve", "trackpoints", "trackstraight", "hole", "shipfront",
    "shipmiddle", "shipback", "bluriterock1", "potteryoven", "potterywheel",
    "crate", "potato", "fishing", "silverrock1", "smashedtable", "nastyfungus",
    "smashedchair", "brokenpillar", "fallentree", "dangersign", "runiterock1",
    "runiteruck1", "largegrave", "curvedbone", "largebone", "carcass",
    "animalskull", "vine", "vinecorner", "vinejunction", "brokenwall",
    "dramentree", "doormat", "rock3", "bigtable", "fireplacea1", "bigegg",
    "eggs", "stalagmites", "stool", "wallbench", "bigroundtable", "roundtable",
    "bush1", "bush2", "blueflower", "smallfern", "giantcrystal", "beehive",
    "marblearch", "obelisk", "sandpit", "oaktree", "willowtree", "mapletree",
    "yewtree", "magictree", "flax", "brokencart", "clockpoleblue",
    "clockpolered", "clockpolepurple", "clockpoleblack", "wallclockface",
    "leverbracket", "metalgateclosed2", "metalgateopen2", "foodtrough",
    "spearwall", "hornedskull", "toplesstree", "catabow", "catabowarrow", "ent",
    "khazardwall", "jungle tree 2", "jungle tree 1", "jungle fern textured",
    "jungle fern textured 2", "jungle fern textured 3",
    "jungle fern textured 4", "jungle fly trap", "jungle fern",
    "jungle spikey fern", "jungle strange plant", "jungle strange plant 2",
    "jungle medium size plant", "jungle statue", "deadtree2", "doubledoorframe",
    "sewervalve", "caveentrance", "logbridgelow", "logbridgehigh",
    "treeplatformhigh", "treeplatformlow", "largetreeplatformlow",
    "largetreeplatformhigh", "logbridgecurvedhigh", "logbridgecurvedlow",
    "treeplatformlow2", "treeplatformhigh2", "lograft", "hazeeltomb",
    "ardoungewall", "ardoungewallcorner", "mudpatch", "mudpile",
    "largesewerpipe", "fishingcrane", "rowboat", "rowboatsinking",
    "fishingcranerot1", "fishingcranerot2", "waterfall", "deadtree2base",
    "elventomb", "waterfalllev1", "waterfalllev2", "stonestand",
    "glarialsstatue", "baxtorianchalice", "baxtorianchalicelow",
    "brokenlograft", "watchtower", "ropeladder", "gallows", "grand tree-lev 0",
    "logbridge lev0", "gnomewatchtower lev0", "logbridgejunction lev0",
    "climbing_rocks", "corner_ledge", "straight_ledge", "log_balance1",
    "log_balance2", "mossyrock", "rocktile", "tribalstature",
    "grand treeinside-lev 0", "grand treeinside-lev 1",
    "grand treeinside-lev 2", "grand tree-lev 1", "grand tree-lev 2",
    "hillsidedoor", "logbridgejunction lev1", "fourwayplatform-lev 0",
    "fourwayplatform-lev 1", "logbridge lev1", "logbridge lev2",
    "gnomewatchtower lev1", "gnomewatchtower lev2", "rockpoolwater",
    "grand tree-lev 3", "blurberrybar", "gemrock", "obstical_ropeswing",
    "obstical_net", "obstical_frame", "tree_for_rope", "tree_with_rope",
    "tree_with_vines", "gnomefence", "beam", "gnomesign", "treeroot1",
    "treeroot2", "gnomecage", "gnomeglider", "gnomeglidercrashed",
    "mudpiledown", "gnomehamek", "gnomegoal", "stonedisc", "obstical_pipe",
    "spikedpit-low", "spikedpit", "bridge section 1", "bridge section 2",
    "bridge section 3", "cave bridge support", "cave platform small",
    "gnomefence2", "bridge section collapsed", "bridge section collapsed2",
    "rams skull door", "rams skull dooropen", "caveentrance2",
    "cave old bridge", "cave old bridgedown", "cave large stagamite",
    "cave small stagamite", "cave rock1", "cave ledge", "cave lever",
    "cave large stagatite", "cave small stagatite",
    "cave extra large stagatite", "cave swampbubbles", "cave rocktrap1",
    "cave rocktrap1a", "cave swamprocks", "cave grilltrap", "cave grilltrapa",
    "cave temple", "cave grilltrapa up", "cave grillcage", "cave grillcageup",
    "cave speartrap", "cave speartrapa", "cave furnace", "cave well",
    "cave tubetrap", "cave tubetrapa", "cave tubetrapa rope", "cave snaptrap",
    "cave snaptrapa", "cave planks", "cave bloodwell",
    "cave platform verysmall", "cave carvings", "cave wallgrill",
    "cave bolder", "cave templedoor", "cave platform small2",
    "cave smashedcage", "cave bridge supportbase", "clawsofiban",
    "bridge section corner", "cave bridge stairs", "cave temple alter",
    "cave templedooropen", "zodiac", "telescope", "cave pillar",
    "dwarf multicannon", "sandyfootsteps", "dwarf multicannon part1",
    "dwarf multicannon part2", "dwarf multicannon part3",
    "small caveentrance2", "signpost2", "brownclimbingrocks", "ogre standard",
    "liftwinch", "rockcounter", "liftbed", "rock cake counter",
    "magearena colomn", "magearena wall", "magearena corner",
    "magearena tallwall", "magearena cornerfill", "magearena tallcorner",
    "magearena plain wall", "spellshock", "magearena door", "cactuswatered",
    "lightning1", "poorbed", "firespell1", "1-1light", "1-1dark", "1-3light",
    "1-3dark", "2-2light", "2-2dark", "barrier1", "halfburiedskeleton",
    "2-1light", "largeurn", "halfburiedskeleton2", "dugupsoil1", "dugupsoil2",
    "dugupsoil3", "sinkingshipfront", "sinkingbarrel", "shipleak", "shipleak2",
    "barrelredcross", "shipspray1", "shipspray2", "ropeforclimbingbot",
    "trawlernet-l", "trawlernet-r", "trawlernet", "totemtree1", "totemtree2",
    "totemtree3", "totemtree4", "totemtree5", "rocksteps", "clawspell1",
    "spellcharge1", "saradominstone", "guthixstone", "zamorakstone",
    "rockpool", "scaffoldsupport", "scaffoldsupportrope", "shamancave",
    "skeletonwithbag", "totemtreeevil", "totemtreegood", "totemtreerotten2",
    "totemtreerotten3", "totemtreerotten4", "totemtreerotten5", "torcha2",
    "torcha3", "torcha4", "skulltorcha2", "skulltorcha3", "skulltorcha4",
    "firea2", "firea3", "fireplacea2", "fireplacea3", "firespell2",
    "firespell3", "lightning2", "lightning3", "clawspell2", "clawspell3",
    "clawspell4", "clawspell5", "spellcharge2", "spellcharge3", NULL
};

int
main(int argc, char **argv)
{
	const char **name;
	char str[64];
	unsigned i, x, y;

	name = file_names;
	do {
		printf("{ .name = \"%s\", .hash = %u },\n",
		    *name, jag_hash_entry_name(*name));
		name++;
	} while (*name != NULL);

	name = media_names;
	do {
		snprintf(str, sizeof(str), "%s.dat", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		snprintf(str, sizeof(str), "%s.tga", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		name++;
	} while (*name != NULL);

	for (i = 0; i < 64; ++i) {
		snprintf(str, sizeof(str), "objects%d.dat", i + 1);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		snprintf(str, sizeof(str), "objects%d.tga", i + 1);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
	}

	name = animation_names;
	do {
		snprintf(str, sizeof(str), "%s.dat", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		snprintf(str, sizeof(str), "%s.tga", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		name++;
	} while (*name != NULL);

	name = texture_names;
	do {
		snprintf(str, sizeof(str), "%s.dat", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		snprintf(str, sizeof(str), "%s.tga", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		name++;
	} while (*name != NULL);

	name = model_names;
	do {
		snprintf(str, sizeof(str), "%s.ob2", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		snprintf(str, sizeof(str), "%s.ob3", *name);
		printf("{ .name = \"%s\", .hash = %u },\n",
		    str, jag_hash_entry_name(str));
		name++;
	} while (*name != NULL);

	for (i = 0; i < 4; ++i) {
		for (x = 47; x < 64; ++x) {
			for (y = 37; y < 56; ++y) {
				snprintf(str, sizeof(str), "m%d%d%d%d%d.jm",
				    i, x / 10, x % 10, y / 10, y % 10);
				printf("{ .name = \"%s\", .hash = %u },\n",
				    str, jag_hash_entry_name(str));
				snprintf(str, sizeof(str), "m%d%d%d%d%d.hei",
				    i, x / 10, x % 10, y / 10, y % 10);
				printf("{ .name = \"%s\", .hash = %u },\n",
				    str, jag_hash_entry_name(str));
				snprintf(str, sizeof(str), "m%d%d%d%d%d.dat",
				    i, x / 10, x % 10, y / 10, y % 10);
				printf("{ .name = \"%s\", .hash = %u },\n",
				    str, jag_hash_entry_name(str));
				snprintf(str, sizeof(str), "m%d%d%d%d%d.loc",
				    i, x / 10, x % 10, y / 10, y % 10);
				printf("{ .name = \"%s\", .hash = %u },\n",
				    str, jag_hash_entry_name(str));
			}
		}
	}

	return EXIT_SUCCESS;
}
