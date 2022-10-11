/*
 * Copyright © 2022 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gio/gio.h>

#include <memory>
#include <string>
#include <cstdlib>

using namespace std::literals;

namespace
{
auto const bamf_prefix = "BAMF_DESKTOP_FILE_HINT=/var/lib/snapd/desktop/applications/"s;

auto extract_id(std::string filename) -> std::string
{
    for (auto& r : filename)
    {
        if (r == '/') r = '-';
    }

    return filename;
}

class Connection : std::shared_ptr<GDBusConnection>
{
public:
    explicit Connection(GDBusConnection* connection) : std::shared_ptr<GDBusConnection>{connection, &g_object_unref} {}

    operator GDBusConnection*() const { return get(); }

private:
    friend void g_object_unref(GDBusConnection*) = delete;
};

void open_desktop_entry(std::string id)
{
    Connection const connection{g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr)};

    static char const* const dest = "io.snapcraft.Launcher";
    static char const* const object_path = "/io/snapcraft/PrivilegedDesktopLauncher";
    static char const* const interface_name = "io.snapcraft.PrivilegedDesktopLauncher";
    static char const* const method_name = "OpenDesktopEntry";

    GError* error = nullptr;

    if (auto const result = g_dbus_connection_call_sync(connection,
                                                        dest,
                                                        object_path,
                                                        interface_name,
                                                        method_name,
                                                        g_variant_new("(s)", id.c_str()),
                                                        nullptr,
                                                        G_DBUS_CALL_FLAGS_NONE,
                                                        G_MAXINT,
                                                        nullptr,
                                                        &error))
    {
        g_variant_unref(result);
    }

    if (error)
    {
        fprintf(stderr, "Dbus error=%s, dest=%s, object_path=%s, interface_name=%s, method_name=%s, id=%s",
                        error->message, dest,    object_path,    interface_name,    method_name,    id.c_str());
        g_error_free(error);
    }
}
}

int main(int argc, char const* argv[])
{
    for (auto i = 0; i != argc; ++i)
    {
        std::string const arg{argv[i]};
        if (arg.starts_with(bamf_prefix))
        {
            open_desktop_entry(extract_id(arg.substr(bamf_prefix.size())));
            exit(EXIT_SUCCESS);
        }
    }
}