// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * Copyright (C) 2015-2019 ICL/ITRI
 * All rights reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of ICL/ITRI and its suppliers, if any.
 * The intellectual and technical concepts contained
 * herein are proprietary to ICL/ITRI and its suppliers and
 * may be covered by Taiwan and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from ICL/ITRI.
 */

#pragma once

#include "base/TypeTraits.h"
#include "base/threads/Thread.h"
#include "base/threads/Types.h"

#include <utility>

// FunctorThread class is an implementation of base Thread interface that
// allows one to run a function object in separate thread. It's mostly a
// convenience class so one doesn't need to create a separate class if the only
// needed thing is to run a specific existing function in a thread.
namespace libeYs3D    {
namespace base {

class FunctorThread : public base::Thread {
public:
    using Functor = base::ThreadFunctor;

    explicit FunctorThread(const Functor& func,
                           ThreadFlags flags = ThreadFlags::MaskSignals)
        : FunctorThread(Functor(func), flags) {}

    explicit FunctorThread(Functor&& func,
                           ThreadFlags flags = ThreadFlags::MaskSignals);

    // A constructor from a void function in case when result isn't important.
    template <class Func, class = enable_if<is_callable_as<Func, void()>>>
    explicit FunctorThread(Func&& func,
                           ThreadFlags flags = ThreadFlags::MaskSignals)
        : Thread(flags), mThreadFunc([func = std::move(func)]() {
              func();
              return intptr_t();
          }) {}

private:
    intptr_t main() override;

    Functor mThreadFunc;
};

}  // namespace base
}  // namespace libeYs3D
