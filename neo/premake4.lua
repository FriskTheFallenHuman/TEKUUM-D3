dofile("premake/embed.lua")

newaction {
	trigger     = "embed",
	description = "Embed RenderProgs in RenderProgs_embedded.h; required before release builds",
	execute     = doembed
}