/**
 * @file analyzer_factory.h
 * @author Chase Geigle
 *
 * All files in META are dual-licensed under the MIT and NCSA licenses. For more
 * details, consult the file LICENSE.mit and LICENSE.ncsa in the root of the
 * project.
 */

#ifndef META_ANALYZER_FACTORY_H_
#define META_ANALYZER_FACTORY_H_

#include "analyzers/analyzer.h"
#include "util/factory.h"
#include "util/shim.h"

namespace cpptoml
{
class toml_group;
}

namespace meta
{
namespace analyzers
{

/**
 * Factory that is responsible for creating analyzers from configuration
 * files.  Clients should use the register_analyzer method instead of this
 * class directly.
 */
class analyzer_factory : public util::factory<analyzer_factory, analyzer,
                                              const cpptoml::toml_group&,
                                              const cpptoml::toml_group&>
{
    /// friend the base class
    friend base_factory;

  private:
    /**
     * Constructor.
     */
    analyzer_factory();

    /**
     * Adds (registers) an analyzer with this factory so it is able to be
     * created.
     */
    template <class Analyzer>
    void register_analyzer();

    /// maps id strings to the factory method used to create that class
    std::unordered_map<std::string, factory_method> methods_;
};

/**
 * Factory method for creating an analyzer. This should be specialized if
 * your given tokenizer requires special construction behavior.
 */
template <class Analyzer>
std::unique_ptr<analyzer> make_analyzer(const cpptoml::toml_group&,
                                        const cpptoml::toml_group&)
{
    return make_unique<Analyzer>();
}

/**
 * Registration method for analyzers. Clients should use this method to
 * register any new filters they write.
 */
template <class Analyzer>
void register_analyzer()
{
    analyzer_factory::get().add(Analyzer::id, make_analyzer<Analyzer>);
}
}
}
#endif
